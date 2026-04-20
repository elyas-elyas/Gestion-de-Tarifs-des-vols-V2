#include <gtest/gtest.h>
#include "../include/FlightNbr.hpp"

/**
 * @brief Verifie l'association entre un vol et son itineraire (avec std::shared_ptr).
 */
TEST(FlightNbrTest, AssociationItineraire) {
    // On cree l'itineraire partage
    auto itineraire = std::make_shared<OD>("CDG", "JFK");
    itineraire->ajouterTarif(Tarif('Y', 1000, 100));

    // On cree le vol
    FlightNbr vol("AF012", itineraire);

    EXPECT_EQ(vol.numeroVol, "AF012");
    ASSERT_NE(vol.itineraire, nullptr); // On verifie que le pointeur n'est pas nul
    EXPECT_EQ(vol.itineraire->origine, "CDG");
}