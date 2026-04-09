#ifndef UNIVERS_HXX
#define UNIVERS_HXX
#include "Particule.hpp"
#include <iostream>
#include <cmath>
#include<vector>


class Univers {
    private :
        int dim = 1;
        int nb_particule;
        std::vector<Particule> particules  ;


    public : 
            Univers(int dim, int nb_part, std::vector<Particule> particules)
                : dim(dim), nb_particule(nb_part), particules(std::move(particules)){}
            Univers() = default;
            void avancer_parts(double dt) {
                for (auto& p : particules) {
                    for (int d = 0; d < dim; ++d) {
                        double new_pos = p.getPosition(d) + p.getVitesse(d) * dt;
                        p.setPosition(d, new_pos);
                    }
                }
            }
            //  afficher l’état de l’univers, i.e. les positions des particules àchaque instant.
            void univers_state() {
                std::cout << "État de l'univers :\n";
                int i = 1;
                for (auto& p : particules) {
                    std::cout << "\nParticule P" << i << " : ";

                    for (int d = 0; d < dim; ++d) {
                        std::cout << p.getPosition(d) << " ";
                    }

                    std::cout << "\n";
                    i++;
                }
            }
            void maj_vitesse(double dt) {
                for (auto& p : particules) {
                    for (int d = 0; d < dim; ++d) {
                        double a = p.getForce(d) / p.getMas();
                        double new_v = p.getVitesse(d) + a * dt;
                        p.setVitesse(d, new_v);
                    }
                }
            }
            // calculs les forces d’interactions
            void all_forces(){
                // RESET 
                for (auto& p : particules) {
                    for (int d = 0; d < dim; ++d) {
                        p.setForce(d, 0.0);
                    }
                }
                for(int i = 0; i < nb_particule ; i++){
                    for(int j= i+1; j < nb_particule ; j++){
                            particules[i].Fij(particules[j]);
                    }
                }
            }




};


#endif