#ifndef FLUIDBOX_H
#define FLUIDBOX_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

using namespace std;
typedef vector<double> VD1;
typedef vector<vector<double>> VD2;
typedef vector<vector<vector<double>>> VD3;

static int const N = 100;
static int const k_gs = 20; // number of iteration in gauss_seidel

class FluidBox {
public:
    FluidBox();
    FluidBox(double visc, double diff);
    // geters
    VD2 get_u();
    VD2 get_v();
    VD2 get_rho();

    // seters 
    void set_viscosity(double v);
    void set_diffusion(double d);

    // static functions
    static VD1 gauss_seidel(VD2 a, VD1 b, VD1 x); 
    static VD2 init_VD2();

    // controls
    void reset();
    
    // update steps
    void forces(VD3 f, double dt);
    void diffusion(double dt);
    void advection();
    void conserve_masse();
    void boundaries_u();
    void boundaries_v();

    void update(VD3 f, double dt);

    // graphics
    void cout();
    void draw(sf::RenderWindow*);

private:
    // coefs
    double visc;
    double diff;

    // x velocity 
    VD2 u;
    VD2 prev_u;
    // y velocity 
    VD2 v;
    VD2 prev_v;

    // density 
    VD2 rho;
    VD2 prev_rho;
};

#endif
