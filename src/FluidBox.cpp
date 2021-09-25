#include "FluidBox.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Vector2.hpp>
#include <math.h>

FluidBox::FluidBox() : FluidBox(init_VD2(0), init_VD2(0), init_VD2(1)) {}

FluidBox::FluidBox(VD2 u0, VD2 v0, VD2 rho0) :
    u(u0), v(v0), rho(rho0),
    u0(u0), v0(v0), rho0(rho0)
{
    prev_u = init_VD2(0);
    prev_v = init_VD2(0);
    prev_rho = init_VD2(1);

    visc = 1;
    diff = 1;
}

VD2 FluidBox::get_u() { return u; }
VD2 FluidBox::get_v() { return v; }
VD2 FluidBox::get_rho() { return rho; }

void FluidBox::set_viscosity(double v) { visc = v; }
void FluidBox::set_diffusion(double d) { diff = d; }

void FluidBox::reset() { u = u0; prev_u = init_VD2(0);
                         v = v0; prev_v = init_VD2(0);
                         rho = rho0; prev_rho = init_VD2(1); }


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
VD2 FluidBox::init_VD2(double x) { return VD2(N+2, VD1(N+2, x)); }

float FluidBox::norm(sf::Vector2f v)
{
    return sqrt(v.x*v.x + v.y*v.y);
}

sf::Vector2f FluidBox::normalize(sf::Vector2f v)
{
    float n = norm(v);
    if (n != 0) {
        return (1 / norm(v)) * v;
    }
    return sf::Vector2f(0,0);
}

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
    VD2 rho0(rho); // deep copy
    for (int k=0; k<k_gs; k++) {
        for (int i=1; i<N+1; i++) {
            for (int j=1; j<N+1; j++) {
                u[i][j] = (u0[i][j] + eff_diff*(u[i-1][j] + u[i+1][j] + 
                                          u[i][j-1] + u[i][j+1])) / (1+4*eff_diff);
                v[i][j] = (v0[i][j] + eff_diff*(v[i-1][j] + v[i+1][j] + 
                                          v[i][j-1] + v[i][j+1])) / (1+4*eff_diff);
                rho[i][j] = (rho0[i][j] + eff_diff*(rho[i-1][j] + rho[i+1][j] + 
                                          rho[i][j-1] + rho[i][j+1])) / (1+4*eff_diff);
            }
        }
        boundaries_u(); // continuity at boundarie
        boundaries_v();
        boundaries_rho();
    }
}

void FluidBox::advection() 
{
   VD2 rho0 = VD2(rho);
   double h = l/N;
   double max = 0;
   
    for (int k=0; k<k_gs; k++) {
        for (int i=1; i<N+1; i++) {
            for (int j=1; j<N+1; j++) {
                double p_pos[2] = {(i+0.5)*h - u[i][j], (j+0.5)*h - v[i][j]};
                double p_id[2] = {floor((p_pos[0] - 0.5*h) / h), floor((p_pos[1] - 0.5*h) / h)};
                double p_off[2] = {p_pos[0]/h - p_id[0]-0.5, p_pos[1]/h - p_id[1]-0.5};
                if (p_id[0] > 0 && p_id[0] < N+1 && p_id[1] > 0 && p_id[1] < N+1) {
                    rho[i][j] = 
                        (rho[p_id[0]][p_id[1]] * (1-p_off[0]) 
                         + rho[p_id[0]][p_id[1]+1] * p_off[0])*(1-p_off[1]) +
                        (rho[p_id[0]+1][p_id[1]] * (1-p_off[0]) 
                         + rho[p_id[0]+1][p_id[1]+1] * p_off[0])*p_off[1] ; 
                }
                
                if (abs(rho[i][j]) > max) { max = abs(rho[i][j]); }
                if (k==0 && i==22 && j==22) {
                    std::cout << "pos: (" << p_pos[0] << ", " << p_pos[1] << ")" << std::endl;
                    std::cout << "id: (" << p_id[0] << ", " << p_id[1] << ")" << std::endl;
                    std::cout << "off: (" << p_off[0] << ", " << p_off[1] << ")" << std::endl;
                }
            }
        }
    }

    std::cout << max << std::endl;
}

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

void FluidBox::boundaries_rho() 
{
    for (int i=1; i<N+1; i++)
    {
        rho[0][i] = rho[1][i];
        rho[N+1][i] = rho[N][i];
        rho[i][0] = rho[i][1];
        rho[i][N+1] = rho[i][N];
    }

    // corners
    rho[0][0] = 0.5*(rho[1][0] + rho[0][1]);
    rho[N+1][0] = 0.5*(rho[N][0] + rho[N+1][1]);
    rho[0][N+1] = 0.5*(rho[1][N+1] + rho[0][N]);
    rho[N+1][N+1] = 0.5*(rho[N+1][N] + rho[N][N+1]);
}

void FluidBox::update(VD3 f, double dt) 
{
    forces(f, dt);
    diffusion(dt);
    advection();
}

void FluidBox::cout(int a, int b) 
{
    std::cout << "u" << std::endl;
    for (int i=a; i<b; i++) {
        for (int j=a; j<b; j++) {
            std::cout << u[j][i] << "  ";
        }
        std::cout << std::endl;
    }

    std::cout << "prev_u" << std::endl;
    for (int i=a; i<b; i++) {
        for (int j=a; j<b; j++) {
            std::cout << prev_u[j][i] << "  ";
        }
        std::cout << std::endl;
    }

    std::cout << "rho" << std::endl;
    for (int i=a; i<b; i++) {
        for (int j=a; j<b; j++) {
            std::cout << rho[j][i] << "  ";
        }
        std::cout << std::endl;
    }
}

void FluidBox::draw(sf::RenderWindow * p_window) 
{
    double off = 100;
    double h = l/N;
    double alpha = 255; // reshape the density to fit the color
    int beta = 5; // reshape the velocity color

    
    sf::RectangleShape dv_int = sf::RectangleShape(sf::Vector2f(h, h));
    dv_int.setOrigin(h/2, h/2);


    for (int i=1; i<N+1; i++) {
        for (int j=1; j<N+1; j++) {
            sf::Vector2f m = sf::Vector2f(off + (i-0.5)*h, off + (j-0.5)*h);

            int color_rho = floor(sqrt(rho[i][j]) * alpha);
            dv_int.setPosition(m);
            dv_int.setFillColor(sf::Color(color_rho, color_rho, color_rho));
            p_window->draw(dv_int);
    
            sf::Vector2f dir = sf::Vector2f(u[i][j], v[i][j]);
            int norm = floor(FluidBox::norm(dir)) * beta;
            dir = FluidBox::normalize(dir);
            if (norm > 255) { norm = 255; }
            sf::Color color_vel = sf::Color(norm, 255-norm, 0);

            sf::Vertex vel[2] = {sf::Vertex(m, color_vel), sf::Vertex(m+ (float)h * dir, color_vel)};
            p_window->draw(vel, 2, sf::Lines);
        }
    }
}
