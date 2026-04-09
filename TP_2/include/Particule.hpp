#ifndef PARTICULE_HPP
#define PARTICULE_HPP

#include <iostream>

enum class Categorie {
    Proton,
    Electron,
    Neutron
};

class Particule {
    private:
        double x = 0.0, y = 0.0;
        double Vx = 0.0, Vy = 0.0;
        double m = 1.0; 
        double Fx = 0.0, Fy = 0.0;
        int Id = 0;
        Categorie Cat = Categorie::Proton;

    public:
        // Constructeur

        Particule(double x, double y, double Vx, double Vy, double m, int Id, Categorie cat, double Fx, double Fy) 
            : x(x), y(y), Vx(Vx), Vy(Vy), m(m), Fx(Fx), Fy(Fy), Id(Id), Cat(cat) {}

        Particule() = default;

        double getMas(){ return m; }
        double getX(){ return x; }
        double getY(){ return y; }

        double getVx(){ return Vx; }
        double getVy(){ return Vy; }

        double getFx(){ return Fx; }
        double getFy(){ return Fy; }

        void setX(double dx){ x = dx; }
        void setY(double dy){ y = dy; }

        void setVx(double v){ Vx = v; }
        void setVy(double v){ Vy = v; }

        void setFx(double f){ Fx = f; }
        void setFy(double f){ Fy = f; }
};

#endif