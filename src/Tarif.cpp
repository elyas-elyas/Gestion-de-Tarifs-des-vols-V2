#include "../include/Tarif.hpp"
#include <iostream>

// Constructeur de la classe Tarif
Tarif::Tarif(char classe, long net, long taxe) 
    : classeReservation(classe), net(net), taxe(taxe) {}

// Obtention de la valeur Net du tarif
long Tarif::getNet() const {
    return net;
}

// Obtention de la valeur de la taxe du tarif
long Tarif::getTaxe() const {
    return taxe;
}

// Affichage des informations du tarif
void Tarif::afficher() const {
    
    std::cout << "Classe : " << classeReservation 
              << " | Net : " << net 
              << " | Taxe : " << taxe 
              << std::endl;
}

// Obtention de la classe de reservation
char Tarif::getClasseReservation() const {
    return classeReservation;
}