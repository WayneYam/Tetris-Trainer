#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include "settings.h"
#include "element.h"
#include "interactable_manager.h"
#include "button.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(600, 800), "SFML works!");


	std::function<void(InteractableManager::Interactable*)> setBlue{
		[](InteractableManager::Interactable* ptr) -> void {
			ptr->setColor(sf::Color::Blue);
		}
	};

	std::function<void(InteractableManager::Interactable*)> setRed{
		[](InteractableManager::Interactable* ptr) -> void {
			ptr->setColor(sf::Color::Red);
		}
	};
	

	std::shared_ptr<Button> testButton = std::make_shared<Button>("Test", InteractableManager::Interactable::nullAction , InteractableManager::Interactable::nullAction, InteractableManager::Interactable::nullAction, setBlue, setRed, 10);

	InteractableManager manager;

	manager.addObj(testButton);

	sf::Texture texture;

	texture.loadFromFile("resources/blockskin.png");

	testButton->setTexture(texture);
	testButton->setTextureRect(sf::IntRect(0, 0, 31, 31));
	
	testButton->setScale(10, 10);
	testButton->setPosition(50.0, 50.0);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

		window.draw(*testButton);
		sf::Vector2i globalPosition = sf::Mouse::getPosition(window);
		
		manager.update(sf::Vector2f(globalPosition.x, globalPosition.y));
        window.display();
    }

    return 0;
}
