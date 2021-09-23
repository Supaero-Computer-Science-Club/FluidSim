#include "FluidBox.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Vector2.hpp>

FluidBox::FluidBox() : FluidBox(init_VD2(), init_VD2(), init_VD2()) {}

FluidBox::FluidBox(VD2 u0, VD2 v0, VD2 rho0) :
    u(u0), v(v0), rho(rho0),
    u0(u0), v0(v0), rho0(rho0)
{
    prev_u = init_VD2();
    prev_v = init_VD2();
    prev_rho = init_VD2();

    visc = 1;
    diff = 1;
}

VD2 FluidBox::get_u() { return u; }
VD2 FluidBox::get_v() { return v; }
VD2 FluidBox::get_rho() { return rho; }

void FluidBox::set_viscosity(double v) { visc = v; }
void FluidBox::set_diffusion(double d) { diff = d; }

void FluidBox::reset() { u = u0; prev_u = init_VD2();
                         v = v0; prev_v = init_VD2();
                         rho = rho0; prev_rho = init_VD2(); }


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

// Return a VD2 with N+2xN+2 0
VD2 FluidBox::init_VD2() { return VD2(N+2, VD1(N+2, 0)); }

// update the velocity of each particules due to external forces, in each direction
void FluidBox::forces(VD3 f, double dt) 
{
    for (int i=0; i<N+2; i++) {
        for (int j=0; j<N+2; j++) {
            u[i][j] += dt*f[0][i][j];
            v[i][j] += dt*f[1][i][j];
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
                u[i][j] = (u0[i][j] + eff_diff*(u[i-1][j] + u[i+1][j] + 
                                          u[i][j-1] + u[i][j+1])) / (1+4*eff_diff);
                v[i][j] = (v0[i][j] + eff_diff*(v[i-1][j] + v[i+1][j] + 
                                          v[i][j-1] + v[i][j+1])) / (1+4*eff_diff);
            }
        }
        boundaries_u(); // continuity at boundarie
        boundaries_v();
    }
}

void FluidBox::advection() {}

void FluidBox::boundaries_u() 
{
    for (int i=1; i<N+1; i++)
    {
        // tang 
        u[0][i] = u[1][i];
        u[N+1][i] = u[N][i];
        
        // ortho (nothing through the box)
        u[i][0] = -u[i][1];
        u[i][N+1] = -u[i][N];
    }

    // corners
    u[0][0] = 0.5*(u[1][0] + u[0][1]);
    u[N+1][0] = 0.5*(u[N][0] + u[N+1][1]);
    u[0][N+1] = 0.5*(u[1][N+1] + u[0][N]);
    u[N+1][N+1] = 0.5*(u[N+1][N] + u[N][N+1]);
}

void FluidBox::boundaries_v() 
{
    for (int i=1; i<N+1; i++)
    {
        // ortho (nothing through the box)
        v[0][i] = -v[1][i];
        v[N+1][i] = -v[N][i];
        
        // tang 
        v[i][0] = v[i][1];
        v[i][N+1] = v[i][N];
    }

    // corners
    v[0][0] = 0.5*(v[1][0] + v[0][1]);
    v[N+1][0] = 0.5*(v[N][0] + v[N+1][1]);
    v[0][N+1] = 0.5*(v[1][N+1] + v[0][N]);
    v[N+1][N+1] = 0.5*(v[N+1][N] + v[N][N+1]);
}

void FluidBox::update(VD3 f, double dt) {}

void FluidBox::cout() 
{
    std::cout << "u" << std::endl;
    for (int i=0; i<N+2; i++) {
        for (int j=0; j<N+2; j++) {
            std::cout << u[j][i] << "  ";
        }
        std::cout << std::endl;
    }

    std::cout << "v" << std::endl;
    for (int i=0; i<N+2; i++) {
        for (int j=0; j<N+2; j++) {
            std::cout << v[j][i] << "  ";
        }
        std::cout << std::endl;
    }
}

void FluidBox::draw(sf::RenderWindow * p_window) 
{
    double l = 800; // size of the box
    double off = 100;
    double width = 2;
    double h = l/N;
    
    sf::RectangleShape dv_int = sf::RectangleShape(sf::Vector2f(h-width, h-width));
    dv_int.setFillColor(sf::Color::Black);
    dv_int.setOrigin((h - width)/2, (h - width)/2);

    sf::RectangleShape dv_ext = sf::RectangleShape(sf::Vector2f(h, h));
    dv_ext.setFillColor(sf::Color::Red);
    dv_ext.setOrigin(h/2, h/2);

    sf::CircleShape test = sf::CircleShape(1);
    test.setFillColor(sf::Color::White);
    test.setOrigin(1, 1);

    for (int i=1; i<N+1; i++) {
        for (int j=1; j<N+1; j++) {
            sf::Vector2f m = sf::Vector2f(off + (i-0.5)*h, off + (j-0.5)*h);
            dv_ext.setPosition(m);
            dv_int.setPosition(m);
            test.setPosition(m);
            p_window->draw(dv_ext);
            p_window->draw(dv_int);
            //p_window->draw(test);

            sf::Vertex vel[2] = {m, m+sf::Vector2f(u[i][j], v[i][j])};
            p_window->draw(vel, 2, sf::Lines);
        }
    }
}
