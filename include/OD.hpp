#ifndef OD_HPP
#define OD_HPP

#include <string>
#include <unordered_map>
#include <optional>
#include <mutex>
#include "Tarif.hpp"

/**
 * @class OD
 * @brief Classe representant un itineraire Origine-Destination.
 */
class OD {
public:
    std::string origine;      ///< Code de l'aeroport d'origine
    std::string destination;  ///< Code de l'aeroport de destination

    /**
     * @brief Constructeur de la classe OD.
     * @param ori Origine de l'itineraire.
     * @param dest Destination de l'itineraire.
     */
    OD(const std::string& ori, const std::string& dest);

    /**
     * @brief Affiche les informations de l'itineraire.
     */
    void afficher() const;

    /**
     * @brief Ajoute ou met a jour un tarif pour cet itineraire (Thread-Safe).
     * @param tarif Objet Tarif a ajouter.
     */
    void ajouterTarif(const Tarif& tarif);

    /**
     * @brief Obtient le tarif net minimum.
     * @return std::optional contenant le tarif minimum s'il existe.
     */
    std::optional<Tarif> obtenirTarifNetMinimum() const;

    /**
     * @brief Obtient le tarif net maximum.
     * @return std::optional contenant le tarif maximum s'il existe.
     */
    std::optional<Tarif> obtenirTarifNetMaximum() const;

    /**
     * @brief Calcule la moyenne des tarifs nets.
     * @return std::optional contenant la moyenne si elle existe.
     */
    std::optional<double> calculerMoyenneNet() const;

private:
    std::unordered_map<char, Tarif> tarifs; ///< Stockage des tarifs optimises en O(1)
    
    mutable std::mutex verrouTarifs; ///< Mutex pour proteger l'acces concurrent aux tarifs
};

#endif