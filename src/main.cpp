#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include "FluidBox.h"

using namespace std;

int main() 
{
    sf::RenderWindow window(sf::VideoMode (1400, 1000), "sfml-app");

    VD2 r = FluidBox::init_VD2(1);
    for (int i=0; i<N+1; i++) {
        for (int j=0; j<N+1; j++) {
            if (i+j <= N) {
                r[i][j] = 0.5;
            }
        }
    }


    VD2 a = FluidBox::init_VD2(0);
    double u = 1000;
    a[20][20] = u;
    a[21][20] = u;
    a[20][21] = u;
    a[21][21] = u;
    a[N-20][N-20] = -u;
    a[N-21][N-20] = -u;
    a[N-20][N-21] = -u;
    a[N-21][N-21] = -u;
    VD3 f = {a, a};//FluidBox::init_VD2(0)};

    FluidBox fbox(FluidBox::init_VD2(0), FluidBox::init_VD2(0), r);
    fbox.set_diffusion(1);

    double dt = 1; // in ms
    bool mode_step = true;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter && mode_step) {
                    fbox.cout(20, 25);
                    fbox.update(f, dt);
                }

                if (event.key.code == sf::Keyboard::Space) {
                    mode_step = !mode_step;
                }

                if (event.key.code == sf::Keyboard::R) {
                    fbox.reset();
                }
            }
        }

        window.clear();

        if (!mode_step) {
            fbox.update(f, dt);
        }

        fbox.draw(&window);
        window.display();
    }
    return 0;
}

