#include "../include/OD.hpp"
#include <iostream>
#include <numeric>

OD::OD(const std::string& ori, const std::string& dest) 
    : origine(ori), destination(dest) {}

void OD::afficher() const {
    std::lock_guard<std::mutex> verrou(verrouTarifs);
    std::cout << "Itineraire : " << origine << " -> " << destination << std::endl;
    if (tarifs.empty()) {
        std::cout << "Aucun tarif disponible." << std::endl;
    } else {
        for (const auto& paire : tarifs) {
            paire.second.afficher();
        }
    }
}

// Ajout optimise en O(1) grace a l'unordered_map (Thread-Safe)
void OD::ajouterTarif(const Tarif& tarif) {
    std::lock_guard<std::mutex> verrou(verrouTarifs);

    char classeRes = tarif.getClasseReservation();
    auto it = tarifs.find(classeRes);

    if (it != tarifs.end()) {
        if (tarif.getNet() < it->second.getNet()) {
            it->second = tarif; 
        }
        else if (tarif.getNet() == it->second.getNet() && tarif.getTaxe() < it->second.getTaxe()) {
            it->second = tarif; 
        }
    } else {
        tarifs.insert({classeRes, tarif});
    }
}

std::optional<Tarif> OD::obtenirTarifNetMinimum() const {
    std::lock_guard<std::mutex> verrou(verrouTarifs);
    if (tarifs.empty()) return std::nullopt;

    auto itMin = tarifs.begin();
    for (auto it = tarifs.begin(); it != tarifs.end(); ++it) {
        if (it->second.getNet() < itMin->second.getNet()) {
            itMin = it;
        }
    }
    return itMin->second;
}

std::optional<Tarif> OD::obtenirTarifNetMaximum() const {
    std::lock_guard<std::mutex> verrou(verrouTarifs);
    if (tarifs.empty()) return std::nullopt;

    auto itMax = tarifs.begin();
    for (auto it = tarifs.begin(); it != tarifs.end(); ++it) {
        if (it->second.getNet() > itMax->second.getNet()) {
            itMax = it;
        }
    }
    return itMax->second;
}

std::optional<double> OD::calculerMoyenneNet() const {
    std::lock_guard<std::mutex> verrou(verrouTarifs);
    if (tarifs.empty()) return std::nullopt;

    long somme = std::accumulate(tarifs.begin(), tarifs.end(), 0L,
        [](long acc, const std::pair<const char, Tarif>& paire) {
            return acc + paire.second.getNet();
        });

    return static_cast<double>(somme) / tarifs.size();
}