#include "../include/FlightNbr.hpp"
#include <iostream>

// Constructeur de la classe FlightNbr.
FlightNbr::FlightNbr(const std::string& numero, std::shared_ptr<OD> od)
    : numeroVol(numero), itineraire(od) {}

// Affiche les informations du vol et ses tarifs.
void FlightNbr::afficher() const {

    std::cout << "Numero de vol : " << numeroVol << std::endl;
    
    // Verifie si un itineraire est associe au vol
    if (itineraire) {
        itineraire->afficher(); // Affiche les tarifs de l'itineraire
    } 
    
    else {
        std::cout << "Aucun itineraire associe." << std::endl;
    }
}