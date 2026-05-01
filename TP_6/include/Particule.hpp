#ifndef PARTICULE_HPP
#define PARTICULE_HPP

#include <iostream>
#include <array>
#include <cmath>


enum class Categorie {
    Proton,
    Electron,
    Neutron
};

class Particule {
    private:
        std::array<double, 2> position; 
        std::array<double, 2> vitesse;
        std::array<double, 2> force;
        double m = 1.0; 
        int Id = 0;
        Categorie Cat = Categorie::Proton;

    public:
        // Constructeur
        Particule(std::array<double, 2> pos, std::array<double, 2> vitess, std::array<double, 2> force, double m, int Id, Categorie cat) 
            : position(pos), vitesse(vitess), force(force), m(m), Id(Id), Cat(cat) {}

        Particule() = default;

        int getDim() const { return 2; } 

        double getPosition(int i) const { return position[i]; }
        double getVitesse(int i) const { return vitesse[i]; }
        double getForce(int i) const { return force[i]; }

        void setPosition(int i, double val) { position[i] = val; }
        void setVitesse(int i, double val) { vitesse[i] = val; }
        void setForce(int i, double val) { force[i] = val; }
        double getMas() const { return m; }
        int getId() const { return Id; }
        // void Fij(const Particule& p2){
        //     double rij = 0;
        //     double m2 =  p2.getMas();
        //     int dim = getDim();
        //     for(int i = 0; i < dim; i++){
        //         double res_int = std::pow(position[i]- p2.getPosition(i), 2);
        //         rij+=res_int;
        //     }
        //     if (rij < 1e-9) return;
        //     double dist  = std::sqrt(rij);
        //     double res = (m2*m)/pow(dist,3);
        //     for (int i = 0; i < dim; i++) {
        //         double f_comp = res * (p2.getPosition(i) - position[i]);
        //         force[i] += f_comp; 
        //     }
        // }

        // La modification repose sur la 3ème loi de Newton : 
        // si la particule i exerce une force sur j, alors j exerce la même force sur i, mais dans le sens opposé 
        void Fij(Particule& p2){
            double rij = 0;
            double m2 =  p2.getMas();
            int dim = getDim();
            for(int i = 0; i < dim; i++){
                double res_int = std::pow(position[i]- p2.getPosition(i), 2);
                rij+=res_int;
            }
            if (rij < 1e-9) return;
            double dist  = std::sqrt(rij);
            double res = (m2*m)/pow(dist,3);
            for (int i = 0; i < dim; i++) {
                double f_comp = res * (p2.getPosition(i) - position[i]);
                force[i] += f_comp; 
                p2.setForce(i, p2.getForce(i) - f_comp);

            }
        }
        // autre optim : L’option -O3 de GCC active un niveau très agressif d’optimisation du code lors de la compilation.
    };

#endif