#include "FluidBox.h"

double** FluidBox::get_u() { return (double **) u; }
double** FluidBox::get_v() { return (double **) v; }
double** FluidBox::get_rho() { return (double **) rho; }

void FluidBox::set_viscosity(double v) { visc = v; }

// update the velocity of each particules due to external forces
void FluidBox::forces(double fu[N+2][N+2], double fv[N+2][N+2], double dt) 
{
    for (int i=0; i<N+2; i++) {
        for (int j=0; j<N+2; j++) {
            u[i][j] += dt*fu[i][j];
            v[i][j] += dt*fv[i][j];
        }
    }
}

void FluidBox::diffusion() {}
void FluidBox::advection() {}

void FluidBox::update(double f[N+2][N+2], double dt) {}

void FluidBox::draw(sf::RenderWindow * p_window) {}
