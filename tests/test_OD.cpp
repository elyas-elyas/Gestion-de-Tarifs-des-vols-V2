#include <gtest/gtest.h>
#include "../include/OD.hpp"

/**
 * @brief Verifie que la creation d'un OD fonctionne.
 */
TEST(ODTest, Initialisation) {
    OD itineraire("CDG", "JFK");
    EXPECT_EQ(itineraire.origine, "CDG");
    EXPECT_EQ(itineraire.destination, "JFK");
}

/**
 * @brief Verifie les regles de remplacement lors de l'ajout d'un tarif.
 */
TEST(ODTest, AjoutEtRemplacementTarif) {
    OD itineraire("CDG", "JFK");
    
    // Ajout initial
    itineraire.ajouterTarif(Tarif('Y', 1000, 100));
    
    // Ce tarif ne doit pas remplacer le premier car son Net (1200) est superieur a 1000
    itineraire.ajouterTarif(Tarif('Y', 1200, 50)); 
    EXPECT_EQ(itineraire.obtenirTarifNetMinimum().value().getNet(), 1000);

    // Ce tarif DOIT remplacer le premier car son Net (800) est inferieur a 1000
    itineraire.ajouterTarif(Tarif('Y', 800, 100));
    EXPECT_EQ(itineraire.obtenirTarifNetMinimum().value().getNet(), 800);

    // Ce tarif DOIT remplacer le precedent car Net (800) est egal, mais la Taxe (50) est inferieure a 100
    itineraire.ajouterTarif(Tarif('Y', 800, 50));
    EXPECT_EQ(itineraire.obtenirTarifNetMinimum().value().getTaxe(), 50);
}

/**
 * @brief Verifie que les fonctions optionnelles gerent bien une liste vide.
 */
TEST(ODTest, GestionListeVide) {
    OD itineraire("ORY", "NCE");
    
    EXPECT_FALSE(itineraire.obtenirTarifNetMinimum().has_value());
    EXPECT_FALSE(itineraire.obtenirTarifNetMaximum().has_value());
    EXPECT_FALSE(itineraire.calculerMoyenneNet().has_value());
}

/**
 * @brief Verifie les calculs de Min, Max et Moyenne.
 */
TEST(ODTest, StatistiquesTarifs) {
    OD itineraire("CDG", "LAX");
    itineraire.ajouterTarif(Tarif('Y', 500, 100));
    itineraire.ajouterTarif(Tarif('J', 1500, 300));
    itineraire.ajouterTarif(Tarif('F', 3000, 500));

    // ASSERT_TRUE verifie qu'il y a bien une reponse (que la liste n'etait pas vide)
    ASSERT_TRUE(itineraire.obtenirTarifNetMinimum().has_value());
    EXPECT_EQ(itineraire.obtenirTarifNetMinimum().value().getNet(), 500);

    ASSERT_TRUE(itineraire.obtenirTarifNetMaximum().has_value());
    EXPECT_EQ(itineraire.obtenirTarifNetMaximum().value().getNet(), 3000);

    ASSERT_TRUE(itineraire.calculerMoyenneNet().has_value());
    // (500 + 1500 + 3000) / 3 = 1666.66...
    EXPECT_NEAR(itineraire.calculerMoyenneNet().value(), 1666.66, 0.01); // On tolere 0.01 de marge d'erreur
}