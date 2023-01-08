#include "settings.h"

void Settings::Init(){ // Initializes Settings
	keybind.resize(kMaxKeybind);
	handling.resize(kMaxHandling);

	// TODO: default settings
}

void Settings::ChangeKeybind(Keybind option, sf::Keyboard::Key newValue){
	keybind[static_cast<int>(option)] = newValue;

	// TODO: check keybind collision
}

void Settings::ChangeHandling(Handling option, HandlingType newValue){
	handling[static_cast<int>(option)] = newValue;	
}

sf::Keyboard::Key Settings::GetKeybind(Keybind option){
	return keybind[static_cast<int>(option)];
}

Settings::HandlingType Settings::GetHandling(Handling option){
	return handling[static_cast<int>(option)];	
}
