#include "Particule.hpp"
#include <iostream>
#include <cmath>
#include <list>
#include <random>
#include <chrono>
#include <vector>



int main() {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> pos(0.0, 4.0);
    std::uniform_real_distribution<double> vitess(1.0, 100.0);
    std::uniform_real_distribution<double> mas(0.1, 1.0);
    std::uniform_int_distribution<int>     cat_dist(0, 2);

    int N;
    std::cout << "Veuillez insérer la taille souhaitée : ";
    std::cin >> N;

    std::list<Particule> particuleList;
    for (int i = 0; i < N; ++i) {
        std::vector<double> p_pos   = {pos(mt),    pos(mt)};
        std::vector<double> p_vit   = {vitess(mt), vitess(mt)};
        std::vector<double> p_force = {0.0,        0.0};
        double m = mas(mt);
        Categorie cat = static_cast<Categorie>(cat_dist(mt));
        particuleList.emplace_back(p_pos, p_vit, p_force, m, i, cat);
    }
    std::cout << "La taille de la liste : " << particuleList.size() << "\n";



    return 0;
}