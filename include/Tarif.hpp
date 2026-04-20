#ifndef TARIF_HPP
#define TARIF_HPP

/**
 * @class Tarif
 * @brief Classe representant un tarif associe a une classe de reservation.
 */
class Tarif {
public:
    /**
     * @brief Constructeur de la classe Tarif.
     * @param classe Caractere representant la classe de reservation.
     * @param net Valeur Net du tarif.
     * @param taxe Montant de la taxe associee.
     */
    Tarif(char classe, long net, long taxe);

    /**
     * @brief Accesseur pour obtenir la classe de reservation.
     * @return Classe de reservation.
     */
    char getClasseReservation() const;

    /**
     * @brief Accesseur pour obtenir la valeur Net du tarif.
     * @return Valeur Net du tarif.
     */
    long getNet() const;

    /**
     * @brief Accesseur pour obtenir la taxe du tarif.
     * @return Valeur de la taxe.
     */
    long getTaxe() const;

    /**
     * @brief Affiche les informations du tarif (classe, Net, Taxe).
     */
    void afficher() const;

private:
    char classeReservation; ///< Classe de reservation 
    long net;  ///< Tarif Net 
    long taxe; ///< Montant de la taxe appliquee
};

#endif