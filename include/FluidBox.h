#ifndef FLUIDBOX_H
#define FLUIDBOX_H

#include <SFML/Graphics.hpp>
#include <vector>

using namespace std;
typedef vector<double> VD1;
typedef vector<vector<double>> VD2;
typedef vector<vector<vector<double>>> VD3;

static int const N = 0;

class FluidBox {
public:
    // geters
    VD2 get_u();
    VD2 get_v();
    VD2 get_rho();

    // seters 
    void set_viscosity(double v);

    // static functions
    static VD1 gauss_seidel(VD2 a, VD1 b, VD1 x); 
    
    // update steps
    void forces(VD3 f, double dt);
    void diffusion();
    void advection();
    void conserve_masse();

    void update(VD3 f, double dt);

    // graphics
    void draw(sf::RenderWindow*);

private:
    // Viscosity coef
    double visc;

    // x velocity 
    VD2 u; // +2 for borders
    // y velocity 
    VD2 v; // +2 for borders

    // density 
    VD2 rho; // +2 for borders
};

#endif
