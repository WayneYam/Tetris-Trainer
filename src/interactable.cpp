#include "interactable.h"
#include <SFML/Window/Mouse.hpp>

bool InteractableManager::Interactable::inBound(sf::Vector2f v){
	return this->getGlobalBounds().contains(v);	
}

