#include "Univers.hxx"
#include <iostream>
#include <vector>
#include <cmath>
#include <random>


int main() {
    int dim = 2;
    double Lx = 10.0, Ly = 10.0, rcut = 2.5;
    int ncx = std::floor(Lx/rcut), ncy = std::floor(Ly/rcut); 

    std::vector<Cellule> mes_cellules;
    for(int y = 0; y < ncy; ++y) {
        for(int x = 0; x < ncx; ++x) {
            mes_cellules.emplace_back(x * rcut, y * rcut);
        }
    }


    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> distPos(0.0, 9.9);

    for(int i  = 0 ; i < 20 ; i++){
        double px = distPos(mt);
        double py = distPos(mt);
        Particule p = Particule({px,py},{px,py},{px,py},px,i,Categorie::Proton);
        int ix = std::floor(px / rcut);
        int iy = std::floor(py / rcut);
        mes_cellules[ix + iy * ncx].ajouterParticule(p);

    }

    Univers mon_univers(dim, 2, Lx, Ly, rcut, std::move(mes_cellules));

    mon_univers.initialiserVoisins(ncx, ncy); 

    std::cout << "--- Avant calcul des forces ---" << std::endl;
    mon_univers.univers_state();

    mon_univers.all_forces(); 

    std::cout << "\n--- Apres calcul des forces ---" << std::endl;
    mon_univers.univers_state();

    return 0;
}


