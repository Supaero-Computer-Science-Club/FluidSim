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

    VD2 u0 = FluidBox::init_VD2();
    u0[20][20] = -5000;
    u0[21][20] = -5000;
    u0[20][21] = -5000;
    u0[21][21] = -5000;

    FluidBox fbox(u0, FluidBox::init_VD2(), FluidBox::init_VD2());
    fbox.cout();

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
                    fbox.diffusion(1e-4);
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
            fbox.diffusion(1e-4);
        }

        fbox.draw(&window);
        window.display();
    }
    return 0;
}

