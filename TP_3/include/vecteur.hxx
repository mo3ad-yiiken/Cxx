#ifndef VECTEUR_HPP
#define VECTEUR_HPP

#include <cmath>
#include <ostream>
#include <iostream> 

class vecteur {

private:
    double x = 0.0, y = 0.0, z = 0.0;

public:

    vecteur(double x, double y, double z) : x(x), y(y), z(z) {}
    vecteur() = default;

    double getX()const{ return x; }
    double getY()const{ return y; }
    double getZ()const{ return z; }

    void setX(double dx){ x = dx; }
    void setY(double dy){ y = dy; }
    void setZ(double dz){ z = dz; }

    vecteur sum_Vectors(vecteur v){
        return vecteur(x + v.x, y + v.y, z + v.z);
    }

    vecteur sub_Vectors(vecteur v){
        return vecteur(x - v.x, y - v.y, z - v.z);
    }

    vecteur multV_par_lambda(double lambda){
        return vecteur(x * lambda, y * lambda, z * lambda);
    }

    double dot(vecteur v){
        return x*v.x + y*v.y + z*v.z;
    }

    double norme(){
        return std::sqrt(x*x + y*y + z*z);
    }
    void print(){
        std::cout << "(" << x << ", "<< y<< ", "<< z<<")" << std::endl;
    }

};

inline std::ostream& operator<<(std::ostream& os, const vecteur& v){
    os << "Vecteur(" << v.getX() << ", " << v.getY() << ", " << v.getZ() << ")";
    return os;
}


#endif