#include "../include/GestionnaireCSV.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <iomanip>
#include <future>
#include <thread>
#include <mutex>
#include <string_view>
#include <charconv> // Necessaire pour std::from_chars (Zero-copy parsing)

std::vector<std::unique_ptr<FlightNbr>> GestionnaireCSV::lireFichierCSV(const std::string& nomFichier) {

    std::vector<std::unique_ptr<FlightNbr>> vols; ///< Liste des vols crees
    std::unordered_map<std::string, std::shared_ptr<OD>> itineraireMap; ///< Associe "Origine-Destination" a un objet OD partage
    std::mutex verrouDictionnaire; ///< Protege l'acces concurrent aux structures de donnees globales

    std::ifstream fichier(nomFichier); ///< Ouvre le fichier CSV en lecture

    if (!fichier.is_open()) {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier " << nomFichier << std::endl;
        return vols;
    }

    // 1. Chargement de toutes les lignes en memoire
    std::vector<std::string> lignesBrutes;
    std::string ligne;
    std::getline(fichier, ligne); // Ignore la premiere ligne (qui est l'en-tete du CSV)
    
    while (std::getline(fichier, ligne)) {
        if (!ligne.empty()) {
            lignesBrutes.push_back(std::move(ligne));
        }
    }
    fichier.close();

    // 2. Configuration du Multithreading
    unsigned int nbCoeurs = std::thread::hardware_concurrency();
    if (nbCoeurs == 0) nbCoeurs = 4; // Valeur par defaut de securite
    
    size_t totalLignes = lignesBrutes.size();
    size_t tailleChunk = totalLignes / nbCoeurs;
    std::vector<std::future<void>> futures;

    // 3. Fonction lambda hyper-optimisee executee par chaque processeur
    auto traiterChunk = [&](size_t indexDebut, size_t indexFin) {
        for (size_t i = indexDebut; i < indexFin; ++i) {
            std::string_view vueLigne = lignesBrutes[i];
            
            // Parsing manuel en std::string_view (Zero-copy)
            size_t posActuelle = 0;
            auto extraireToken = [&](std::string_view& token) {
                size_t posVirgule = vueLigne.find(',', posActuelle);
                token = vueLigne.substr(posActuelle, posVirgule - posActuelle);
                posActuelle = (posVirgule == std::string_view::npos) ? std::string_view::npos : posVirgule + 1;
            };

            std::string_view numVolVue, origineVue, destinationVue, classeResVue, netVue, taxeVue;
            extraireToken(numVolVue);
            extraireToken(origineVue);
            extraireToken(destinationVue);
            extraireToken(classeResVue);
            extraireToken(netVue);
            extraireToken(taxeVue);

            // Conversion ultra-rapide des nombres sans instanciation de std::string (C++17)
            long net = 0, taxe = 0;
            std::from_chars(netVue.data(), netVue.data() + netVue.size(), net);
            std::from_chars(taxeVue.data(), taxeVue.data() + taxeVue.size(), taxe);

            // Creation des cles standards
            std::string origine(origineVue);
            std::string destination(destinationVue);
            std::string numeroVol(numVolVue);
            std::string cleItineraire = origine + "-" + destination;

            std::shared_ptr<OD> itineraire;

            // Section critique : Verifie si l'itineraire existe deja
            {
                std::lock_guard<std::mutex> verrou(verrouDictionnaire);
                auto it = itineraireMap.find(cleItineraire);
                if (it == itineraireMap.end()) {
                    itineraire = std::make_shared<OD>(origine, destination);
                    itineraireMap[cleItineraire] = itineraire;
                } else {
                    itineraire = it->second;
                }
            }

            // Ajoute le tarif a l'itineraire (la methode dans OD.cpp est desormais protegee par son propre mutex)
            itineraire->ajouterTarif(Tarif(classeResVue[0], net, taxe));

            // Section critique : Verifie si le vol existe deja pour eviter les doublons
            {
                std::lock_guard<std::mutex> verrou(verrouDictionnaire);
                bool volExistant = false;
                for (const auto& vol : vols) {
                    if (vol->numeroVol == numeroVol) {
                        volExistant = true;
                        break;
                    }
                }
                
                // Si le vol n'existe pas encore, l'ajoute a la liste avec make_unique
                if (!volExistant) {
                    vols.push_back(std::make_unique<FlightNbr>(numeroVol, itineraire));
                }
            }
        }
    };

    // 4. Distribution de la charge sur les coeurs du processeur
    for (unsigned int t = 0; t < nbCoeurs; ++t) {
        size_t debut = t * tailleChunk;
        size_t fin = (t == nbCoeurs - 1) ? totalLignes : (t + 1) * tailleChunk;
        futures.push_back(std::async(std::launch::async, traiterChunk, debut, fin));
    }

    // 5. Synchronisation de tous les threads
    for (auto& f : futures) {
        f.get();
    }

    std::cout << "[INFO] Parsage Zero-Copy termine via " << nbCoeurs << " threads." << std::endl;
    return vols;
}

void GestionnaireCSV::exporterRapportCSV(const std::vector<std::unique_ptr<FlightNbr>>& vols, const std::string& nomFichier) {

    std::ofstream fichier(nomFichier); ///< Ouvre le fichier CSV en ecriture
    
    if (!fichier.is_open()) {
        std::cerr << "Erreur : Impossible de creer le fichier " << nomFichier << std::endl;
        return;
    }

    // Ecriture de l'en-tete du fichier CSV
    fichier << "Numero de vol,Origine,Destination,Classe Min,Net Min,Taxe Min,"
               "Classe Max,Net Max,Taxe Max,Moyenne Net\n";

    // Parcours des vols et ecriture des valeurs dans le fichier
    for (const auto& vol : vols) {
        if (!vol->itineraire) continue;

        // Recuperation des std::optional
        auto tarifMinOpt = vol->itineraire->obtenirTarifNetMinimum();
        auto tarifMaxOpt = vol->itineraire->obtenirTarifNetMaximum();
        auto moyenneNetOpt = vol->itineraire->calculerMoyenneNet();

        // Verification que les valeurs existent bien avant d'ecrire (remplace l'ancien try/catch)
        if (tarifMinOpt && tarifMaxOpt && moyenneNetOpt) {
            
            // Extraction des valeurs contenues dans les std::optional avec .value()
            Tarif tarifMin = tarifMinOpt.value();
            Tarif tarifMax = tarifMaxOpt.value();
            double moyenneNet = moyenneNetOpt.value();

            // Utilisation du getter getClasseReservation() au lieu de l'attribut prive
            fichier << vol->numeroVol << ","
                    << vol->itineraire->origine << ","
                    << vol->itineraire->destination << ","
                    << tarifMin.getClasseReservation() << ","
                    << tarifMin.getNet() << ","
                    << tarifMin.getTaxe() << ","
                    << tarifMax.getClasseReservation() << ","
                    << tarifMax.getNet() << ","
                    << tarifMax.getTaxe() << ","
                    << std::fixed << std::setprecision(2) << moyenneNet
                    << "\n";
        }
    }

    fichier.close();
    std::cout << "Rapport genere : " << nomFichier << std::endl;
}