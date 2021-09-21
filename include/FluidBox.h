#ifndef FLUIDBOX_H
#define FLUIDBOX_H

int static const N = 100;

class FluidBox {
public:
    // geters
    double** get_u();
    double** get_v();
    double** get_rho();

    // seters 
    void set_viscosity(double v);
    
    // update steps
    void forces(double f[N+2][N+2]);
    void diffusion();
    void advection();
    void conserve_masse();

    void update(double f[N+2][N+2]);

private:
    // Viscosity coef
    double visc;

    // x velocity 
    double u[N+2][N+2]; // +2 for borders
    // y velocity 
    double v[N+2][N+2]; // +2 for borders

    // density 
    double rho[N+2][N+2]; // +2 for borders
};

#endif
