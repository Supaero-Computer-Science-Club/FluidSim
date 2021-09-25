#ifndef FLUIDBOX_H
#define FLUIDBOX_H

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>
#include <iostream>

using namespace std;
typedef vector<double> VD1;
typedef vector<vector<double>> VD2;
typedef vector<vector<vector<double>>> VD3;

static int const N = 50;
static int const k_gs = 20; // number of iteration in gauss_seidel

class FluidBox {
public:
    FluidBox();
    FluidBox(VD2 u0, VD2 v0, VD2 rho0);
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
    static VD2 init_VD2(double x);
    static float norm(sf::Vector2f);
    static sf::Vector2f normalize(sf::Vector2f);

    // controls
    void reset();
    
    // update steps
    void forces(VD3 f, double dt);
    void diffusion(double dt);
    void advection();
    void conserve_masse();
    void boundaries_u();
    void boundaries_v();
    void boundaries_rho();

    void update(VD3 f, double dt);

    // graphics
    void cout(int a, int b);
    void draw(sf::RenderWindow*);

private:
    // coefs
    double visc;
    double diff;

    // size
    double const l = 800; // size of the box

    // x velocity 
    VD2 u;
    VD2 u0;
    VD2 prev_u;
    // y velocity 
    VD2 v;
    VD2 v0;
    VD2 prev_v;

    // density 
    VD2 rho;
    VD2 rho0;
    VD2 prev_rho;
};

#endif
