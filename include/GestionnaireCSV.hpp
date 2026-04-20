#ifndef GESTIONNAIRECSV_HPP
#define GESTIONNAIRECSV_HPP

#include "FlightNbr.hpp"
#include <vector>
#include <string>
#include <memory>

/**
 * @class GestionnaireCSV
 * @brief Classe utilitaire pour la gestion de l'importation et de l'exportation de fichiers CSV.
 */
class GestionnaireCSV {
public:
    /**
     * @brief Lit un fichier CSV contenant des informations sur les vols et tarifs puis cree les objets FlightNbr et OD correspondants.
     * @param nomFichier Nom du fichier CSV a lire.
     * @return Un vecteur de pointeurs intelligents uniques vers les objets FlightNbr crees.
     */
    static std::vector<std::unique_ptr<FlightNbr>> lireFichierCSV(const std::string& nomFichier);

    /**
     * @brief Exporte un rapport CSV contenant les informations analytiques.
     * @param vols Liste des vols a inclure dans le rapport.
     * @param nomFichier Nom du fichier CSV a generer.
     */
    static void exporterRapportCSV(const std::vector<std::unique_ptr<FlightNbr>>& vols, const std::string& nomFichier);
};

#endif