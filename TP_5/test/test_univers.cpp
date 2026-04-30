#include <gtest/gtest.h>
#include "Univers.hxx" 
TEST(CelluleTest, GestionParticules) {
    Cellule c(0.0, 0.0);
    EXPECT_EQ(c.nbParticules(), 0);

    std::vector<double> pos = {1.0, 1.0};
    std::vector<double> vit = {0.0, 0.0};
    std::vector<double> force = {0.0, 0.0};
    Particule p(pos, vit, force, 1.0, 1, Categorie::Proton);

    c.ajouterParticule(p);
    EXPECT_EQ(c.nbParticules(), 1);

    c.vider();
    EXPECT_EQ(c.nbParticules(), 0);
}

TEST(UniversTest, MiseAJourVitesse) {
    std::vector<double> pos = {0.0, 0.0};
    std::vector<double> vit = {0.0, 0.0};
    std::vector<double> force = {4.0, 0.0};
    Particule p(pos, vit, force, 2.0, 1, Categorie::Proton); 

    Cellule c(0.0, 0.0);
    c.ajouterParticule(p);
    std::vector<Cellule> liste_cellules = {c};

    Univers u(2, 1, 10.0, 10.0, 2.5, liste_cellules);


    u.maj_vitesse(0.5);

    Particule p_maj = u.getCellules()[0][0]; 
    EXPECT_DOUBLE_EQ(p_maj.getVitesse(0), 1.0);
    EXPECT_DOUBLE_EQ(p_maj.getVitesse(1), 0.0);
}

TEST(UniversTest, AvancerParticules) {
    std::vector<double> pos = {1.0, 2.0};
    std::vector<double> vit = {3.0, -1.0};
    std::vector<double> force = {0.0, 0.0};
    Particule p(pos, vit, force, 1.0, 1, Categorie::Proton);

    Cellule c(0.0, 0.0);
    c.ajouterParticule(p);
    std::vector<Cellule> liste_cellules = {c};

    Univers u(2, 1, 10.0, 10.0, 2.5, liste_cellules);

    u.avancer_parts(2.0);

    Particule p_maj = u.getCellules()[0][0];
    EXPECT_DOUBLE_EQ(p_maj.getPosition(0), 7.0);
    EXPECT_DOUBLE_EQ(p_maj.getPosition(1), 0.0);
}