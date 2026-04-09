# C++ pour les Mathématiques Appliquées — TPs 1 à 4

> Grenoble INP — MMIS  
> Cours : C++ pour les Mathématiques Appliquées 

## Description

Implémentation progressive d'un simulateur de dynamique moléculaire en C++.


## Structure du projet

```text
├── CMakeLists.txt       
├── README.md            
├── doc/
│   ├── CMakeLists.txt   
│   ├── analyse.txt      
│   └── doxyfilerc       
├── include/
│   ├── Particule.hpp    
│   └── Univers.hxx      
├── src/
│   ├── CMakeLists.txt
│   └── Univers.cxx      
├── test/
│   ├── CMakeLists.txt
│   └── test_forces.cxx  
├── demo/
│   ├── CMakeLists.txt
│   └── demo.cpp         
└── build/               
```

## TPs

| TP | Contenu |
|----|---------|
| TP1 | Introduction C++, prise en main |
| TP2 | Classe `Particule` : encapsulation, constructeurs, getters/setters |
| TP3 | Opérateurs, calcul de forces (Newton 3), intégrateur Störmer-Verlet |
| TP4 | Découpage de l'espace en cellules, maillage, calcul de forces avec rayon de coupure, mise à jour des cellules |

## Fonctionnalités implémentées

- Classe `Particule` avec position, vitesse, force en dimension quelconque
- Calcul de force gravitationnelle / Lennard-Jones entre paires
- Optimisation 3ème loi de Newton (`Fij = -Fji`)
- Maillage en cellules (`Cellule`, `Univers`)
- Initialisation des cellules voisines
- Calcul de forces avec rayon de coupure `rcut`
- Mise à jour des cellules après déplacement (`maj_cellules`)
- Simulation de collision de deux objets (Q6)

## Compilation

```bash
g++ -O3 -Wall -Wextra -Iinclude demo/demo.cpp -o Simulation
./Simulation
```

## Visualisation



## Paramètres de la simulation (TP4 Q6)

- Domaine : `Lx=250`, `Ly=120`
- `σ=1`, `ε=1`, `rcut=2.5`
- `dt=0.00005`, `tend=19.5`
- Distance inter-particules : `d0 = 2^(1/6)`
- Carré rouge : 40×40 particules, vitesse initiale `(0, -10)`
- Rectangle bleu : 160×40 particules, vitesse initiale `(0, 0)`
