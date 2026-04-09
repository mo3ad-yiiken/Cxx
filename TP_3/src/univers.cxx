#include "Univers.hxx"
#include "Particule.hpp"
#include <iostream>
#include <cmath>
#include <vector>
#include <random>
#include <chrono>

int main() {
    int dim = 3;
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    for (int k = 3; k <= 7; ++k) {
        int n = std::pow(2, k);
        int N = std::pow(n, 3);
        
        std::vector<Particule> list_particules;
        list_particules.reserve(N);

        for(int i = 0; i < N ; i++){
            std::vector<double> pos = {dist(mt), dist(mt), dist(mt)};
            std::vector<double> v = {1.0, 1.0, 1.0};
            std::vector<double> F = {0.0, 0.0, 0.0};
            list_particules.emplace_back(pos, v, F, 1.0, i, Categorie::Proton);
        }

        Univers mon_univers(dim, N, std::move(list_particules));

        auto start = std::chrono::steady_clock::now();
        
        mon_univers.all_forces();
        
        auto end = std::chrono::steady_clock::now();
        
        std::chrono::duration<double> elapsed_seconds = end - start;
        std::cout << "k=" << k << " (N=" << N << ") elapsed time: " << elapsed_seconds.count() << "s" << std::endl;

        if (elapsed_seconds.count() > 60.0) break;
    }

    return 0;
}