#include "interactable_manager.h"
#include <iostream>
#include <SFML/Window/Mouse.hpp>

std::shared_ptr<InteractableManager::Interactable> InteractableManager::query(sf::Vector2f position){
	
	int maxVal = 0;
	std::shared_ptr<InteractableManager::Interactable> ans = nullptr;
	for(auto obj : this->objs){
		int currVal = obj->active * obj->inBound(position) * obj->priority; 
		if(currVal > maxVal){
			maxVal = currVal;
			ans = obj;
		}
	}

	return ans;
}

void InteractableManager::update(sf::Vector2f v){
	
	bool mouse = sf::Mouse::isButtonPressed(sf::Mouse::Left);
	auto ptr = query(v);

	for(auto obj : this->objs){
		bool inside = (obj == ptr);

		if(obj->state == Interactable::kNull){
			if(inside && (!mouse) ){
				obj->hover();
				obj->state = Interactable::kHover;
			}
		}else if(obj->state == Interactable::kHover){
			if(!inside){
				obj->leave();
				obj->state = Interactable::kNull;
			}else if(mouse){
				obj->click();
				obj->state = Interactable::kPress;
			}
		}else{
			if(!mouse){
				obj->release();
				if(inside){
					obj->state = Interactable::kHover;
				}else{
					obj->leave();
					obj->state = Interactable::kNull;
				}
			}else{
				obj->press();
			}
		}

	}

}
