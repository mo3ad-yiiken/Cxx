#include "Particule.hpp"
#include "Univers.hxx"
#include <iostream>
#include <cmath>
#include <chrono>
#include <vector>
#include <fstream>



void Stormer_Verlet(Univers& univers, int nb_part, int dim, double tend, double dt) {
    auto collecter = [&]() {
        std::vector<Particule*> ptrs;
        for (Cellule& c : univers.getCellules())
            for (Particule& p : c)
                ptrs.push_back(&p);
        return ptrs;
    };

    std::vector<std::vector<double>> Fold(nb_part, std::vector<double>(dim, 0.0));
    univers.all_forces();

    double t = 0.0;
    int step = 0;


    while (t < tend) {
        t += dt;
        step++;

        auto ptrs = collecter();
        for (size_t i = 0; i < ptrs.size(); ++i) {
            Particule& p = *ptrs[i];
            double mi = p.getMas();
            for (int k = 0; k < dim; ++k) {
                double xk  = p.getPosition(k);
                double vk  = p.getVitesse(k);
                double Fik = p.getForce(k);
                p.setPosition(k, xk + dt * (vk + 0.5 / mi * Fik * dt));
                Fold[i][k] = Fik;
            }
        }

        univers.maj_cellules();
        univers.all_forces();

        ptrs = collecter();
        for (size_t i = 0; i < ptrs.size(); ++i) {
            Particule& p = *ptrs[i];
            double mi = p.getMas();
            for (int k = 0; k < dim; ++k) {
                double vk  = p.getVitesse(k);
                double Fik = p.getForce(k);
                p.setVitesse(k, vk + dt * 0.5 / mi * (Fik + Fold[i][k]));
            }
        }
    }
}
