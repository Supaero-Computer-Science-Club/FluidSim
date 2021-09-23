#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include "FluidBox.h"

using namespace std;

int main() 
{
    sf::RenderWindow window(sf::VideoMode (1000, 1000), "sfml-app");

    FluidBox fbox;
    cout << "Hello, World!" << endl;
    
    vector<VD2> f = {FluidBox::init_VD2(), FluidBox::init_VD2()};
    for (int i=0; i<N+2; i++) { f[1][i][1] = 1e7; }
    fbox.set_diffusion(1);
    fbox.forces(f, 1e-4);
    fbox.cout();
    fbox.diffusion(1e-4);
    fbox.cout();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        fbox.diffusion(1e-4);
        fbox.draw(&window);
        window.display();
    }
    return 0;
}

