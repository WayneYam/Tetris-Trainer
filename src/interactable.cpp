#include "interactable.h"

bool Interactable::inBound(sf::Vector2f v){
	return this->getGlobalBounds().contains(v);	
}

