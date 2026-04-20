#ifndef FLIGHTNBR_HPP
#define FLIGHTNBR_HPP

#include <string>
#include <vector>
#include <memory>
#include "OD.hpp"

/**
 * @class FlightNbr
 * @brief Classe representant un vol avec un numero et un itineraire associe.
 */
class FlightNbr {
public:
    std::string numeroVol; ///< Numero du vol
    std::shared_ptr<OD> itineraire; ///< Pointeur intelligent partage vers l'itineraire correspondant au vol

    /**
     * @brief Constructeur de la classe FlightNbr.
     * @param numero Numero du vol.
     * @param od Pointeur intelligent partage vers l'itineraire correspondant.
     */
    FlightNbr(const std::string& numero, std::shared_ptr<OD> od);

    /**
     * @brief Affiche les informations du vol et ses tarifs associes.
     */
    void afficher() const;
};

#endif