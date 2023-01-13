#include <SFML/Graphics.hpp>
#include <vector>
#include "settings.h"

int main()
{
	

    sf::RenderWindow window(sf::VideoMode(600, 800), "SFML works!");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.display();
    }

    return 0;
}
