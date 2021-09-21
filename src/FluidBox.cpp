#include "FluidBox.h"

double** FluidBox::get_u() { return (double **) u; }
double** FluidBox::get_v() { return (double **) v; }
double** FluidBox::get_rho() { return (double **) rho; }

void FluidBox::set_viscosity(double v) { visc = v; }


void FluidBox::forces(double f[N+2][N+2]) {}
void FluidBox::diffusion() {}
void FluidBox::advection() {}

void FluidBox::update(double f[N+2][N+2]) {}
