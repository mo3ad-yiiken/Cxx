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

void Stormer_Verlet(Univers& univers, int nb_part, int dim, double tend, double dt) {
    // std::vector<Type> nom(taille, valeur_par_defaut);
    std::vector<std::vector<double>> Fold(nb_part, std::vector<double>(dim, 0.0));

    univers.all_forces();
    
    double t = 0.0;
    int step = 0;

    sauvegarderVTK("frames/frame_0.vtu",univers);
    while (t < tend) {
        t += dt;
        step++;

        auto ptrs = collecterToutesLesParticules(univers);
            for (Particule* ptr : ptrs) {
            Particule& p = *ptr;
            int i = p.getId(); 
            double mi = p.getMas();
            
            for (int k = 0; k < dim; ++k) {
                double xi = p.getPosition(k);
                double vi = p.getVitesse(k);
                double Fi = p.getForce(k);
                p.setPosition(k, xi + dt * (vi + 0.5 / mi * Fi * dt));
                Fold[i][k] = Fi;
            }
        } 
        univers.maj_cellules();
        univers.all_forces();

        ptrs = collecterToutesLesParticules(univers);
        for (Particule* ptr : ptrs) {
            Particule& p = *ptr;
            int i = p.getId(); 
            double mi = p.getMas();
            
            for (int k = 0; k < dim; ++k) {
                double vi = p.getVitesse(k);
                double Fi = p.getForce(k);
                double Fold_i = Fold[i][k];
                p.setVitesse(k, vi + dt * 0.5 / mi * (Fi + Fold_i));
            }
        } 

        // 
        if (step % 5 == 0) {
            std::cout << "Progression t = " << t << " / " << tend << "\r" << std::flush;
            sauvegarderVTK("frames/frame_" + std::to_string(step) + ".vtu",univers);
        }
    } 
    sauvegarderVTK("etat_final.vtu",univers);
    std::cout << "\nSimulation terminee a t=" << t << "\n";
}

int main() {
    const int    dim    = 2;
    const double Lx     = 250.0;
    const double Ly     = 120.0;
    const double rcut   = 2.5;
    const double dt     = 0.005; 
    const double tend   = 8.0;   
    const double sigma  = 1.0;
    const double d0     = std::pow(2.0, 1.0/6.0) * sigma;

    int ncd_x = std::max(1, static_cast<int>(Lx / rcut));
    int ncd_y = std::max(1, static_cast<int>(Ly / rcut));
    double cell_dx = Lx / ncd_x;
    double cell_dy = Ly / ncd_y;

    int nb_part = 40*40 + 160*40;
    Univers univers(dim, nb_part, Lx, Ly, rcut);


    int id = 0;

    double carre_x0 = (Lx - 39.0 * d0) / 2.0;
    double carre_y0 = 75.0;
    for (int j = 0; j < 40; ++j)
        for (int i = 0; i < 40; ++i) {
            Particule p({carre_x0 + i*d0, carre_y0 + j*d0},
                        {0.0, -10}, {0.0, 0.0},
                        1.0, id++, Categorie::Proton);
            univers.ajouterParticule(p);
        }

    double rect_x0 = (Lx - 159.0 * d0) / 2.0;
    double rect_y0 = 10.0;
    for (int j = 0; j < 40; ++j)
        for (int i = 0; i < 160; ++i) {
            Particule p({rect_x0 + i*d0, rect_y0 + j*d0},
                        {0.0, 0.0}, {0.0, 0.0},
                        1.001, id++, Categorie::Proton);
            univers.ajouterParticule(p);
        }

    std::cout << "Particules placees : " << id << "\n";

    auto start = std::chrono::steady_clock::now();
    Stormer_Verlet(univers, nb_part, dim, tend, dt);
    auto end_t = std::chrono::steady_clock::now();
    std::cout << "Elapsed: " << std::chrono::duration<double>(end_t-start).count() << "s\n";
    return 0;
}