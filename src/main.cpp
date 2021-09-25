#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
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

    VD2 r = FluidBox::init_VD2(0);
    for (int i=15; i<=35; i++) {
        r[i][15] = 1;
        r[i][35] = 1;
        r[15][i] = 1;
        r[35][i] = 1;
    }


    VD2 a = FluidBox::init_VD2(0);
    a[2][25] = 10;
    VD3 f = {VD2(a), FluidBox::init_VD2(0)};

    FluidBox fbox(FluidBox::init_VD2(0), FluidBox::init_VD2(0), r);
    fbox.set_diffusion(1e-3);

    bool mode_step = true;
    
    sf::Clock clock;

    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();
        cout << "dt " << dt << endl;

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

