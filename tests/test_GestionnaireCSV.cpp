#include <gtest/gtest.h>
#include "../include/GestionnaireCSV.hpp"
#include <fstream>
#include <cstdio> // Pour std::remove

/**
 * @brief Test de la gestion CSV. 
 * Note: Ce test ecrit un mini-fichier CSV temporaire pour etre independant.
 */
TEST(GestionnaireCSVTest, LectureEtEcriture) {
    // 1. Preparation : Creation d'un faux fichier CSV temporaire
    std::string fichierEntree = "test_temp_input.csv";
    std::ofstream out(fichierEntree);
    out << "NumeroVol,Origine,Destination,ClasseReservation,Net,Taxe\n";
    out << "AF123,CDG,JFK,Y,1000,100\n";
    out << "AF123,CDG,JFK,J,2000,200\n";
    out.close();

    // 2. Execution : Lecture du fichier
    auto vols = GestionnaireCSV::lireFichierCSV(fichierEntree);

    // 3. Verification de la lecture
    ASSERT_EQ(vols.size(), 1); // Il ne doit y avoir qu'un seul vol (AF123) car les 2 lignes concernent le meme
    EXPECT_EQ(vols[0]->numeroVol, "AF123");
    
    // 4. Execution : Exportation
    std::string fichierSortie = "test_temp_output.csv";
    GestionnaireCSV::exporterRapportCSV(vols, fichierSortie);

    // 5. Verification que le fichier d'export existe
    std::ifstream in(fichierSortie);
    EXPECT_TRUE(in.good());
    in.close();

    // 6. Nettoyage : Suppression des fichiers temporaires
    std::remove(fichierEntree.c_str());
    std::remove(fichierSortie.c_str());
}