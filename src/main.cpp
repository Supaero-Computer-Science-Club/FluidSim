#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "FluidBox.h"

using namespace std;

int main() 
{
    sf::RenderWindow window(sf::VideoMode (1000, 1000), "sfml-app");

    FluidBox fbox;
    cout << "Hello, World!" << endl;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        fbox.draw(&window);
        window.display();
    }
    return 0;
}

