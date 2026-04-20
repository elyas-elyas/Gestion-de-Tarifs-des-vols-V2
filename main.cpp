#include "../include/GestionnaireCSV.hpp"
#include <iostream>
#include <memory>
#include <vector>
#include <string>

/**
 * @brief Affiche un résumé statistique global des vols traités.
 * * @param vols Liste des vols analysés.
 */
void afficherTableauDeBord(const std::vector<std::unique_ptr<FlightNbr>>& vols) {
    std::cout << "\n=========================================\n";
    std::cout << "      TABLEAU DE BORD - YIELD MANAGEMENT     \n";
    std::cout << "=========================================\n";
    
    if (vols.empty()) {
        std::cout << "Aucune donnee a analyser.\n";
        return;
    }

    int totalVols = vols.size();
    long tarifMinGlobal = 9999999; // Initialisé avec une valeur très haute
    long tarifMaxGlobal = 0;
    std::string volMoinsCher, volPlusCher;

    // Parcours de tous les vols pour trouver les extrêmes
    for (const auto& vol : vols) {
        if (!vol->itineraire) continue;
        
        auto minOpt = vol->itineraire->obtenirTarifNetMinimum();
        if (minOpt && minOpt.value().getNet() < tarifMinGlobal) {
            tarifMinGlobal = minOpt.value().getNet();
            volMoinsCher = vol->numeroVol + " (" + vol->itineraire->origine + " -> " + vol->itineraire->destination + ")";
        }

        auto maxOpt = vol->itineraire->obtenirTarifNetMaximum();
        if (maxOpt && maxOpt.value().getNet() > tarifMaxGlobal) {
            tarifMaxGlobal = maxOpt.value().getNet();
            volPlusCher = vol->numeroVol + " (" + vol->itineraire->origine + " -> " + vol->itineraire->destination + ")";
        }
    }

    std::cout << "Total des vols traites : " << totalVols << "\n";
    std::cout << "Vol le moins cher      : " << volMoinsCher << " a " << tarifMinGlobal << " EUR Net\n";
    std::cout << "Vol le plus cher       : " << volPlusCher << " a " << tarifMaxGlobal << " EUR Net\n";
    std::cout << "=========================================\n\n";
}

/**
 * @brief Main du projet.
 * * @param argc Nombre d'arguments en ligne de commande.
 * @param argv Valeurs des arguments.
 * @return Retourne 0 si l'exécution est réussie.
 */
int main(int argc, char* argv[]) {
    // Parametres par defaut si aucun argument n'est fourni
    std::string inputFile = "data/sample.csv";
    std::string outputFile = "data/rapport_analyse.csv";

    // Gestion des arguments en ligne de commande (Override)
    if (argc == 3) {
        inputFile = argv[1];
        outputFile = argv[2];
    } else if (argc != 1) {
        std::cerr << "[ERREUR] Usage: " << argv[0] << " <fichier_entree.csv> <fichier_sortie.csv>\n";
        std::cerr << "Exemple: " << argv[0] << " data/sample.csv data/rapport_analyse.csv\n";
        return 1;
    }

    std::cout << "[INFO] Lancement de l'analyse tarifaire...\n";
    std::cout << "[INFO] Fichier source cible : " << inputFile << "\n";

    // Lecture du fichier CSV
    std::vector<std::unique_ptr<FlightNbr>> vols = GestionnaireCSV::lireFichierCSV(inputFile);

    if (vols.empty()) {
        std::cerr << "[ERREUR] L'analyse a echoue, aucune donnee valide trouvee dans le fichier.\n";
        return 1;
    }

    // Exportation du rapport
    GestionnaireCSV::exporterRapportCSV(vols, outputFile);
    
    // Affichage du tableau de bord executif
    afficherTableauDeBord(vols);

    return 0;
}