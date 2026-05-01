#include "Univers.hxx"
#include <iostream>
#include <vector>
#include <cmath>
#include <random>


int main() {

    // Univers u(2, 1, 10.0, 10.0, 2.5);
    // Particule p({9.8, 5.0}, {1.0, 0.5}, {0.0, 0.0}, 1.0, 1, Categorie::Proton);
    // u.ajouterParticule(p);
    // for (int i = 0; i < 5; ++i) {
    //     u.avancer_parts(0.1);
    //     u.appliquer_reflection();
    //     u.univers_state();
    // }
    // u.univers_state();

    // // Test absorption
    // Univers u2(2, 1, 10.0, 10.0, 2.5);
    // u2.ajouterParticule(p);
    // for (int i = 0; i < 5; ++i) {
    //     u2.avancer_parts(0.1);
    //     u2.appliquer_absorption();
    //     u2.univers_state(); // doit disparaître après quelques steps
    // }
    // u2.univers_state(); 

    // // Test périodique
    // Univers u3(2, 1, 10.0, 10.0, 2.5);
    // u3.ajouterParticule(p);
    // for (int i = 0; i < 5; ++i) {
    //     u3.avancer_parts(0.1);
    //     u3.appliquer_periodique();
    //     u3.univers_state(); // réapparaît de l'autre côté
    // }

    // Réflexion géométrique
    Univers u1(2, 1, 10.0, 10.0, 2.5);
    Particule p({9.5, 5.0}, {1.0, 0.0}, {0.0, 0.0}, 1.0, 1, Categorie::Proton);
    u1.ajouterParticule(p);
    for (int i = 0; i < 10; ++i) {
        u1.avancer_parts(0.1);
        u1.appliquer_reflection();
        u1.univers_state(i);
    }

    // Réflexion potentiel
    Univers u2(2, 1, 10.0, 10.0, 2.5);
    Particule p2({8.0, 5.0}, {1.0, 0.0}, {0.0, 0.0}, 1.0, 1, Categorie::Proton);
    u2.ajouterParticule(p2);
    for (int i = 0; i < 30; ++i) {
        u2.all_forces();
        u2.potentiel_reflex();
        u2.maj_vitesse(0.1);
        u2.avancer_parts(0.1);
        u2.univers_state(i);
    }
    // Réflexion géométrique : inversion instantanée de la vitesse au contact du bord, position corrigée brutalement.
    //Potentiel : la particule ralentit progressivement avant le bord, repart doucement — plus physique mais la vitesse finale (-0.99) est légèrement inférieure à la vitesse initiale (1.0) → perte d'énergie due à la discrétisation en temps (dt trop grand).
    //Les deux approches donnent le même comportement macroscopique mais le potentiel est plus réaliste physiquement.
    
    return 0;
}


