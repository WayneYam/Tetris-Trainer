#include "settings.h"

void Settings::changeKeybind(Keybind option, sf::Keyboard::Key newValue){
	this->keybind[static_cast<int>(option)] = newValue;

	// TODO: check keybind collision
}

void Settings::changeHandling(Handling option, HandlingType newValue){
	this->handling[static_cast<int>(option)] = newValue;	
}

sf::Keyboard::Key Settings::getKeybind(Keybind option){
	return this->keybind[static_cast<int>(option)];
}

Settings::HandlingType Settings::getHandling(Handling option){
	return this->handling[static_cast<int>(option)];	
}
