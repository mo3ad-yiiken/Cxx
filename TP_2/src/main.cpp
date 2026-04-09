#include "Particule.hpp"
#include <iostream>
#include <cmath>
#include <list>
#include <random>
#include <chrono>
#include <vector>
#include <fstream>

double dist(Particule p1, Particule p2){
    double dx = p2.getX() - p1.getX();
    double dy = p2.getY() - p1.getY();
    return std::sqrt(dx*dx+dy*dy);
}

double FIG(Particule p1, Particule p2){
    double mi=p1.getMas();
    double mj= p2.getMas();
    double rij = dist(p1,p2);
    return (mi*mj)/(rij*rij);
}

void CalculeF(std::vector<Particule>& particules) {
    for (auto& p : particules) {
        p.setFx(0.0);
        p.setFy(0.0);
    }
    for (int i = 0; i < particules.size(); ++i) {
        for (int j = i + 1; j < particules.size(); ++j) {
            Particule& p1 = particules[i];
            Particule& p2 = particules[j];
            double dx = p2.getX() - p1.getX();
            double dy = p2.getY() - p1.getY();
            double r2 = dx*dx + dy*dy + 1e-9;
            double r = std::sqrt(r2);
            double f = (p1.getMas() * p2.getMas()) / (r2 * r);
            double fx = f * dx;
            double fy = f * dy;
            p1.setFx(p1.getFx() + fx);
            p1.setFy(p1.getFy() + fy);
            p2.setFx(p2.getFx() - fx);
            p2.setFy(p2.getFy() - fy);
        }
    }
}

void Stormer_Verlet(std::vector<Particule>& particules, std::vector<double>& Fold) {
    double tend = 468.5;
    double dt = 0.015;
    double t = 0.0;
    std::ofstream fichier("resultats.txt");
    CalculeF(particules);

    while (t < tend) {
        t = t + dt;
        for (int i = 0; i < particules.size(); i++) {
            Particule& p = particules[i];
            p.setX(p.getX() + dt * (p.getVx() + 0.5 / p.getMas() * p.getFx() * dt));
            p.setY(p.getY() + dt * (p.getVy() + 0.5 / p.getMas() * p.getFy() * dt));
            Fold[2*i]   = p.getFx();
            Fold[2*i+1] = p.getFy();
        }
        CalculeF(particules);
        for (int i = 0; i < particules.size(); i++) {
            Particule& p = particules[i];
            p.setVx(p.getVx() + dt * 0.5 / p.getMas() * (p.getFx() + Fold[2*i]));
            p.setVy(p.getVy() + dt * 0.5 / p.getMas() * (p.getFy() + Fold[2*i+1]));
        }
        for (int i = 0; i < particules.size(); i++) {
            Particule& p = particules[i];
            fichier << t << " " << p.getX() << " " << p.getY() << std::endl;
        }
    }
    fichier.close();
}

int main() {
    std::vector<Particule> particules;

    particules.push_back(Particule(0.0, 0.0, 0.0, 0.0, 1.0, 0, Categorie::Proton, 0.0, 0.0));
    particules.push_back(Particule(0.0, 1.0, -1.0, 0.0, 3.0e-6, 1, Categorie::Proton, 0.0, 0.0));
    particules.push_back(Particule(0.0, 5.36, -0.425, 0.0, 9.55e-4, 2, Categorie::Proton, 0.0, 0.0));
    particules.push_back(Particule(34.75, 0.0, 0.0, 0.0296, 1.0e-14, 3, Categorie::Proton, 0.0, 0.0));

    std::cout << "Nombre de particules : " << particules.size() << std::endl;

    std::vector<double> Fold(particules.size() * 2, 0.0);

    auto start = std::chrono::steady_clock::now();
    Stormer_Verlet(particules, Fold);
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "elapsed time: " << elapsed_seconds.count() << "s" << std::endl;

    return 0;
}