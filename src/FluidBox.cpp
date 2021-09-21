#include "FluidBox.h"

VD2 FluidBox::get_u() { return u; }
VD2 FluidBox::get_v() { return v; }
VD2 FluidBox::get_rho() { return rho; }

void FluidBox::set_viscosity(double v) { visc = v; }

// Static functions

// Gauss Seidel 
// Iterative algorithm to solve the linear system ax = b
// The output, y, is equal to the next iteration of x
VD1 FluidBox::gauss_seidel(VD2 a, VD1 b, VD1 x) 
{   
    VD1 y(N+2, 0); // deep copy
    for (int i=0; i<N+2; i++)
    {
        y[i] = b[i]/a[i][i];
        for (int j=0; j<i; j++) {
            y[i] -= a[i][j]*y[j]/a[i][i]; 
        }
        for (int j=i+1; j<N+2; j++) {
            y[i] -= a[i][j]*x[j]/a[i][i]; 
        }
    }
    return y;
}

// update the velocity of each particules due to external forces, in each direction
void FluidBox::forces(VD3 f, double dt) 
{
    for (int k=0; k < f.size(); k++) 
    {
        for (int i=0; i<N+2; i++) {
            for (int j=0; j<N+2; j++) {
                u[i][j] += dt*f[k][i][j];
                v[i][j] += dt*f[k][i][j];
            }
        }
    }
}

void FluidBox::diffusion(double dt) 
{
    // effective diffusion coef, resized with the dimension of the box and the deltatime
    double eff_diff = N*N*diff*dt; 

    VD2 u0(u); // deep copy
    VD2 v0(v); // deep copy
    for (int k=0; k<k_gs; k++) {
        for (int i=1; i<N+1; i++) {
            for (int j=1; j<N+1; j++) {
                u[i][j] += (u0[i][j] + eff_diff*(u[i-1][j] + u[i+1][j] + 
                                          u[i][j-1] + u[i][j+1])) / (1+4*eff_diff);
                v[i][j] += (v0[i][j] + eff_diff*(v[i-1][j] + v[i+1][j] + 
                                          v[i][j-1] + v[i][j+1])) / (1+4*eff_diff);
            }
        }
        boundaries_u(); // continuity at boundarie
        boundaries_v();
    }
}

void FluidBox::advection() {}

void FluidBox::update(VD3 f, double dt) {}

void FluidBox::draw(sf::RenderWindow * p_window) {}
