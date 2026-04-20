#include <gtest/gtest.h>
#include "../include/Tarif.hpp"

/**
 * @brief Test de l'initialisation et des getters de la classe Tarif.
 */
TEST(TarifTest, InitialisationCorrecte) {
    Tarif tarif('Y', 1500, 200);

    // Verifie que les getters retournent bien les valeurs donnees au constructeur
    EXPECT_EQ(tarif.getClasseReservation(), 'Y');
    EXPECT_EQ(tarif.getNet(), 1500);
    EXPECT_EQ(tarif.getTaxe(), 200);
}