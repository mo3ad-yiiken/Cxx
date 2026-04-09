#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>


int sigma = 1, epsilon = 1;
double P_Lennard_Jones(double rij){
    double res = 4*(std::pow(sigma/rij,6))*(std::pow(sigma/rij,6)-1);
    return res;
}

int main(){
    std::vector<double> potentiel_tab;
    std::ofstream file("data.txt");

    for(int i = 1; i < 11 ; i++){
        potentiel_tab.push_back(P_Lennard_Jones(i));
        std::cout << potentiel_tab[i-1] <<"\n";
        file << i << " " << potentiel_tab[i-1] <<"\n";

    }
    file.close();

    system("python dessin.py data.txt");

    return 0;
}