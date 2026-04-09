
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;


double phi(double x, double u) {
    return 2.0 * x * u;
}
double phi_bis(double x, double u) {
    return -50.0 * (u - cos(x));
}



void Euler_ex(int nb_iter, int u0, double h ,  double (*f)(double, double)  ){
    
    vector<double> res(nb_iter + 1);
    double x = 0.0;
    res[0] = u0;

    for (int i = 0; i < nb_iter; i++) {
        res[i+1] = res[i] + h * f(x, res[i]);
        x += h;
    }

    x = 0.0;
    for (int i = 0; i <= nb_iter; i++) {
        cout << "x = " << x
             << "  u_Euler_ex = " << res[i]
             << endl;
        x += h;
    }
}

void Euler_implicite(int nb_iter, int u0, double h ,  double (*f)(double, double)  ){
    
    vector<double> res(nb_iter + 1);
    double x = 0.0;
    res[0] = u0;

    for (int i = 0; i < nb_iter; i++) {
        res[i+1] = res[i] + h * f(x+h, res[i+1]);
        x += h;
    }

    x = 0.0;
    for (int i = 0; i <= nb_iter; i++) {
        cout << "  u_exact_imp = " << res[i]
             << endl;
        x += h;
    }
}

int main() {

    int nb_iter = 20;
    double h = 0.1;
    double u0 = 0;

    auto ex = Euler_ex(nb_iter, u0, h, phi_bis);
    auto imp = Euler_implicite(nb_iter, u0, h);

    double x = 0.0;
    for (int i = 0; i < nb_iter; i++) {
        cout << "x" << x
             << "Euler explicite " << ex[i]
             << "Euler implicite " << imp[i]
        x += h;
    }


    return 0;
}
