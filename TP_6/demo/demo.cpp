#include "Particule.hpp"
#include "Univers.hxx"
#include <iostream>
#include <cmath>
#include <chrono>
#include <vector>
#include <fstream>
#include <string>


std::vector<Particule*> collecterToutesLesParticules(Univers& univers) {
    
    std::vector<Particule*> liste_pointeurs;

    for (Cellule& ma_cellule : univers.getCellules()) {
            for (Particule& ma_particule : ma_cellule) {
            liste_pointeurs.push_back(&ma_particule);
        }
    }
    return liste_pointeurs;
}

void sauvegarderVTK(const std::string& nom, Univers& univ) {
    std::ofstream out(nom);
    if (!out.is_open()) {
        throw std::runtime_error("Erreur : Impossible d'ouvrir ou de créer le fichier " + nom + ".");
    }
    std::vector<Particule*> ptrs = collecterToutesLesParticules(univ);
    int nbPart = ptrs.size();

    out << "<VTKFile type=\"UnstructuredGrid\" version=\"0.1\" byte_order=\"LittleEndian\">\n";
    out << "  <UnstructuredGrid>\n";
    out << "    <Piece NumberOfPoints=\"" << nbPart << "\" NumberOfCells=\"0\">\n";
    
    out << "      <Points>\n";
    out << "        <DataArray Name=\"Points\" type=\"Float32\" NumberOfComponents=\"3\" format=\"ascii\">\n";
    for (Particule* ptr : ptrs) {
        out << ptr->getPosition(0) << " " << ptr->getPosition(1) << " 0 ";
    }
    out << "\n        </DataArray>\n";
    out << "      </Points>\n";

    out << "      <PointData Vectors=\"Velocity\">\n";
    
    out << "        <DataArray Name=\"Velocity\" type=\"Float32\" NumberOfComponents=\"3\" format=\"ascii\">\n";
    for (Particule* ptr : ptrs) {
        out << ptr->getVitesse(0) << " " << ptr->getVitesse(1) << " 0 ";
    }
    out << "\n        </DataArray>\n";

    out << "        <DataArray Name=\"Masse\" type=\"Float32\" format=\"ascii\">\n";
    for (Particule* ptr : ptrs) {
        out << ptr->getMas() << " ";
    }
    out << "\n        </DataArray>\n";

    out << "      </PointData>\n";

    out << "      <Cells>\n";
    out << "        <DataArray type=\"Int32\" Name=\"connectivity\" format=\"ascii\"></DataArray>\n";
    out << "        <DataArray type=\"Int32\" Name=\"offsets\" format=\"ascii\"></DataArray>\n";
    out << "        <DataArray type=\"UInt8\" Name=\"types\" format=\"ascii\"></DataArray>\n";
    out << "      </Cells>\n";

    out << "    </Piece>\n";
    out << "  </UnstructuredGrid>\n";
    out << "</VTKFile>\n";
}

void Stormer_Verlet(Univers& univers, int nb_part, int dim, double tend, double dt, double Ec_cible) {
    // std::vector<Type> nom(taille, valeur_par_defaut);
    std::vector<double> Fold(nb_part * dim, 0.0);

    univers.all_forces();
    
    double t = 0.0;
    int step = 0;

    sauvegarderVTK("frames/frame_0.vtu",univers);
    while (t < tend) {
        t += dt;
        step++;
        for (Cellule& ma_cellule : univers.getCellules()) {
            for (Particule& p : ma_cellule) {
                int i = p.getId(); 
                double mi = p.getMas();
                
                for (int k = 0; k < dim; ++k) {
                    double xi = p.getPosition(k);
                    double vi = p.getVitesse(k);
                    double Fi = p.getForce(k);
                    p.setPosition(k, xi + dt * (vi + 0.5 / mi * Fi * dt));
                    Fold[i * dim + k] = Fi;
                }
            } 
        }
        univers.maj_cellules();
        univers.all_forces();

        for (Cellule& ma_cellule : univers.getCellules()) {
            for (Particule& p : ma_cellule) {
                int i = p.getId(); 
                double mi = p.getMas();
                
                for (int k = 0; k < dim; ++k) {
                    double vi = p.getVitesse(k);
                    double Fi = p.getForce(k);
                    double Fold_i = Fold[i * dim + k];
                    p.setVitesse(k, vi + dt * 0.5 / mi * (Fi + Fold_i));
                }
            } 
        }

        // 
        if (step % 5 == 0) {
            std::cout << "Progression t = " << t << " / " << tend << "\r" << std::flush;
            for (Cellule& ma_cellule : univers.getCellules()) {
                for (Particule& p : ma_cellule) {
                    if (std::isnan(p.getVitesse(0)) || std::isnan(p.getPosition(0))) {
                        std::cout << "NAN detecte step=" << step << " id=" << p.getId() << "\n";
                        return;
                    }
                }
            }
            sauvegarderVTK("frames/frame_" + std::to_string(step) + ".vtu",univers);
        }
        if (step % 1000 == 0)
            univers.rescaleV(Ec_cible);
        } 
    sauvegarderVTK("etat_final.vtu",univers);
    std::cout << "\nSimulation terminee a t=" << t << "\n";
}

int main() {

    try{    
        const int    dim    = 2;
        const double Lx     = 250.0;
        const double Ly     = 180.0;
        const double rcut   = 2.5;
        const double dt     = 0.005; 
        const double tend   = 29.5;   
        const double sigma  = 1.0;
        const double d0     = std::pow(2.0, 1.0/6.0) * sigma;
        // const int N1 = 395;
        // const int N2 = 17227;
        // const double Ec_D = 0.005 * (N1 + N2) ; 

        // int ncd_x = std::max(1, (int)(Lx / rcut));
        // int ncd_y = std::max(1, (int)(Ly / rcut));
        // double cell_dx = Lx / ncd_x;
        // double cell_dy = Ly / ncd_y;

        int nb_part = 40*40 + 160*40;
        Univers univers(dim, nb_part, Lx, Ly, rcut);


        int id = 0;
        double R = 15.0 * d0;
        double cx = Lx / 2.0;
        double cy = 120.0;
        for (int j = 0; j < 40; ++j){
            for (int i = 0; i < 40; ++i) {
                double x = cx + i*d0 - 20*d0, y = cy + j*d0 - 20*d0; // Pour centrer la grille de 40x40 autour de centre_x on pars du centre_x - 20*d0 pour que le centre de la grille soit bien à centre_x.
                double dx = x - cx, dy = y - cy;
                if (dx*dx + dy*dy <= R*R){
                    Particule p({x, y}, {0.0, -10}, {0.0, 0.0},1.0, id++, Categorie::Proton);
                    univers.ajouterParticule(p);
                }
            }
        }

        double n = (int)(Lx/d0);
        for (int j = 1; j < 40; ++j)
            for (int i = 0; i < n; ++i) {
                Particule p({i*d0, j*d0},
                            {0.0, 0.0}, {0.0, 0.0},
                            1.001, id++, Categorie::Proton);
                univers.ajouterParticule(p);
            }

        std::cout << "Particules placees : " << id << "\n";
        const double Ec_D = 0.005 * id;
        auto start = std::chrono::steady_clock::now();
        int nb_partc = id;
        Stormer_Verlet(univers, nb_partc, dim, tend, dt, Ec_D);
        auto end_t = std::chrono::steady_clock::now();
        std::cout << "Elapsed: " << std::chrono::duration<double>(end_t-start).count() << "s\n";
    }
    catch (const std::exception& e) {
        std::cerr << "\n[ ARRET DE LA SIMULATION ]\n";
        std::cerr << "Raison : " << e.what() << "\n";
        return 1;
    }
    return 0;
}