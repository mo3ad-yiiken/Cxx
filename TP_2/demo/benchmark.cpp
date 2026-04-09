#include "Particule.hpp"
#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include <chrono>
#include <random>

int main() {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> val(0.0, 10.0);

    int tailles[] = {64, 128, 256, 512, 1024, 2048};

    for (int N : tailles) {
        std::cout << "N = " << N << std::endl;

        // vector
        std::vector<Particule> vec;
        auto start = std::chrono::steady_clock::now();
        for (int i = 0; i < N; i++) {
            vec.push_back(Particule(val(mt), val(mt), val(mt),
                val(mt), val(mt), i, Categorie::Proton, 0.0, 0.0));
        }
        double s = 0;
        for (int i = 0; i < vec.size(); i++) {
            s += vec[i].getX();
        }
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> t1 = end - start;
        std::cout << "  vector : " << t1.count() << "s" << std::endl;

        // list
        std::list<Particule> lis;
        start = std::chrono::steady_clock::now();
        for (int i = 0; i < N; i++) {
            lis.push_back(Particule(val(mt), val(mt), val(mt),
                val(mt), val(mt), i, Categorie::Proton, 0.0, 0.0));
        }
        s = 0;
        for (auto it = lis.begin(); it != lis.end(); ++it) {
            s += it->getX();
        }
        end = std::chrono::steady_clock::now();
        std::chrono::duration<double> t2 = end - start;
        std::cout << "  list   : " << t2.count() << "s" << std::endl;

        // deque
        std::deque<Particule> deq;
        start = std::chrono::steady_clock::now();
        for (int i = 0; i < N; i++) {
            deq.push_back(Particule(val(mt), val(mt), val(mt),
                val(mt), val(mt), i, Categorie::Proton, 0.0, 0.0));
        }
        s = 0;
        for (int i = 0; i < deq.size(); i++) {
            s += deq[i].getX();
        }
        end = std::chrono::steady_clock::now();
        std::chrono::duration<double> t3 = end - start;
        std::cout << "  deque  : " << t3.count() << "s" << std::endl;

        std::cout << std::endl;
    }

    return 0;
}