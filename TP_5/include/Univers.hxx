#ifndef UNIVERS_HXX
#define UNIVERS_HXX
#include "Particule.hpp"
#include <iostream>
#include <cmath>
#include<vector>


// Univers devra donc contenir une liste de Cellule. Chacune de ces Cellule contient des particules
// et possède des Cellules voisines (3 en 1D, 9 en 2D, 27 en 3D).
class Cellule{
    private:
        double cx, cy;                       
        int nb_voisins =9;
        std::vector<Particule> particules_internes;        
        std::vector<int> indices_voisines;

    public:
        Cellule() : cx(0.0), cy(0.0) {}
        Cellule(double x, double y) : cx(x), cy(y) {}
        double getCx() const { return cx; }
        double getCy() const { return cy; }
        void setNbVoisin(int dim){
            nb_voisins = std::pow(3, dim);
        }
        void ajouterVoisin(int index) {
            indices_voisines.push_back(index);
        }
        int getNbVoisins() const{
            return nb_voisins;
        }
        void ajouterParticule(const Particule& p) {
            particules_internes.push_back(p);
        }
        int nbParticules() const { 
            return particules_internes.size(); 
        }

        Particule& operator[](int i) { // surcharger l'opérateur[]
            return particules_internes[i];  // renvoie une réf (Particule&)
        }
        const std::vector<int>& getIndicesVoisines() const { 
            return indices_voisines; 
        }
        void vider() {
            particules_internes.clear();
        }
        // itérateurs
        auto begin()  {
            return particules_internes.begin(); 
        }
        auto end()  { 
            return particules_internes.end(); 
        }
};

class Grille {
    private:
        int nx, ny;//, nz;           // Nombre de cellules par axe
        double taille_cellule;    // Taille d'un côté d'un cube
        std::vector<Cellule> cellules;
    public :
        Grille() : nx(1.0), ny(1.0), taille_cellule(3.0) {}
        Grille(int x, int y, double taille): nx(x), ny(y), taille_cellule(taille){
            cellules.resize(x*y);
            for (int j = 0; j < y; ++j)
                for (int i = 0; i < x; ++i)
                    cellules[i + j * x] = Cellule((i + 0.5) * taille, (j + 0.5) * taille);
        }
        int getNx() const { return nx; }
        int getNy() const { return ny; }

        int getIndex(int x, int y) const {
            return x + nx*y ;
        }
        Cellule& getCellule(int x, int y) {
            return cellules[getIndex(x, y)];
        }
        std::vector<Cellule>& getCellules(){return cellules;}
        void initialiserVoisins() {
            for (int y = 0; y < ny; ++y) {
                for (int x = 0; x < nx; ++x) {
                    int currentIdx = x + y * nx;
                    
                    // On boucle de -1 à +1 autour de la cellule (x, y)
                    for (int dy = -1; dy <= 1; ++dy) {
                        for (int dx = -1; dx <= 1; ++dx) {
                            int nx_voisin = x + dx;
                            int ny_voisin = y + dy;

                            if (nx_voisin >= 0 && nx_voisin < nx && ny_voisin >= 0 && ny_voisin < ny) {
                                int voisinIdx = nx_voisin + ny_voisin * nx;
                                cellules[currentIdx].ajouterVoisin(voisinIdx);
                            }
                        }
                    }
                }
            }
    }
};




class Univers { 
    private :
        int dim = 2;
        int sigma = 1, epsilon = 5;
        int nb_particule;
        double Lx, Ly;
        double rcut;
        Grille grille;
        std::vector<Particule*> particules;


    public : 
           Univers(int dim, int nb_part, double Lx, double Ly, double rcut)
            : dim(dim), nb_particule(nb_part), Lx(Lx), Ly(Ly), rcut(rcut)
            {
                int ncd_x   = std::max(1, (int)(Lx / rcut));
                int ncd_y   = std::max(1, (int)(Ly / rcut));
                grille = Grille(ncd_x, ncd_y, rcut);
                grille.initialiserVoisins();
            }//move :on copie juste quelques pointeurs internes
            Univers() = default;
            void avancer_parts(double dt) {
                for(auto& c : grille.getCellules()){
                    for (auto& p : c) {
                        for (int d = 0; d < dim; ++d) {
                            double new_pos = p.getPosition(d) + p.getVitesse(d) * dt;
                            p.setPosition(d, new_pos);
                        }
                    }
                }
            }
            std::vector<Cellule>& getCellules() { 
                return grille.getCellules(); 
            }
            //  afficher l’état de l’univers, i.e. les positions des particules àchaque instant.
            void univers_state() {
                std::cout << "État de l'univers :\n";
                int i = 1;
                for(auto& c : grille.getCellules() ){
                    for (auto& p : c) {
                        std::cout << "\nParticule P" << i << "(";

                        for (int d = 0; d < dim; ++d) {
                            if (d != dim -1 ){
                            std::cout << p.getForce(d) << ", ";
                            }
                            else{
                                std::cout << p.getForce(d);
                            }
                        }
                        std::cout << ")\n";
                        i++;
                    }
                }
            }
            void maj_vitesse(double dt) {
                for(Cellule& c : grille.getCellules() ){
                    for (auto& p : c) {
                        for (int d = 0; d < dim; ++d) {
                            double a = p.getForce(d) / p.getMas();
                            double new_v = p.getVitesse(d) + a * dt;
                            p.setVitesse(d, new_v);
                        }
                    }
                }
            }


            double P_Lennard_Jones(double rij){
                double res = 4*(std::pow(sigma/rij,6))*(std::pow(sigma/rij,6)-1);
                return res;
            }


            void all_forces() {
                auto& cellules = grille.getCellules();
                for (Cellule& c : cellules) {
                    for (Particule& p : c) { 
                        for (int d = 0; d < dim; ++d) p.setForce(d, 0.0);
                    }
                }

                for (Cellule& celluleA : cellules) {
                    for (Particule& pI : celluleA) { 
                        
                        for (int idxVoisin : celluleA.getIndicesVoisines()) {
                            Cellule& cellVoisine = cellules[idxVoisin];
                            
                            double dxC = pI.getPosition(0) - cellVoisine.getCx();
                            double dyC = pI.getPosition(1) - cellVoisine.getCy();
                            double distCentre = std::sqrt(dxC*dxC + dyC*dyC);
                            
                            if (distCentre <= rcut) { 
                                
                                for (Particule& pJ : cellVoisine) {
                                    if (&pI == &pJ) continue; 

                                    double dx = pI.getPosition(0) - pJ.getPosition(0);
                                    double dy = pI.getPosition(1) - pJ.getPosition(1);
                                    double r2 = dx*dx + dy*dy;
                                    double r = std::sqrt(r2);

                                    if (r <= rcut) { 
                                        double s_r6 = std::pow(sigma / r, 6);
                                        double intensite = (24.0 * epsilon / r2) * s_r6 * (2.0 * s_r6 - 1.0);                                         
                                        pI.setForce(0, pI.getForce(0) + intensite * dx);
                                        pI.setForce(1, pI.getForce(1) + intensite * dy);
                                    }
                                }
                            }
                        }
                    }
                }
            }   
            // version naiive !! REMEMBER to update it chef
            void maj_cellules() {
                double cell_dx = Lx / grille.getNx();
                double cell_dy = Ly / grille.getNy();
                auto& cellules = grille.getCellules();
                std::vector<Particule> toutes;
                for (Cellule& c : cellules)
                    for (Particule& p : c)
                        toutes.push_back(p);

                for (Cellule& c : cellules)
                    c.vider();
                for (auto& p : toutes) {
                    int cx = std::max(0, std::min((int)(p.getPosition(0) / cell_dx), grille.getNx()-1));
                    int cy = std::max(0, std::min((int)(p.getPosition(1) / cell_dy), grille.getNy()-1));
                    grille.getCellule(cx, cy).ajouterParticule(p);
                }
            }

            int getNbPart() { return nb_particule; }
            void ajouterParticule(const Particule& p) {
                double cell_dx = Lx / grille.getNx();
                double cell_dy = Ly / grille.getNy();

                int cx = std::max(0, std::min((int)(p.getPosition(0) / cell_dx), grille.getNx() - 1));
                int cy = std::max(0, std::min((int)(p.getPosition(1) / cell_dy), grille.getNy() - 1));

                grille.getCellule(cx, cy).ajouterParticule(p);
            }
            
};






#endif