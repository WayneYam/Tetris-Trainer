#ifndef SETTINGS_H
#define SETTINGS_H

#include <vector>
#include <SFML/Window.hpp>

class Settings{

public:
	typedef int HandlingType;
	enum Keybind{ // keybinds of the game

		kMoveLeft,
		kMoveRight,
		kSoftDrop,
		kHardDrop,
		kRotationCW,  // Clockwise Rotation
		kRotationCCW, // Counter-Clockwise Rotation
		kRotation180,
		kHold,

		kMaxKeybind
	};

	enum Handling{
		kDAS, // Delayed Auto Shift
		kARR, // Auto Repeat Rate
		kSDF, // Soft Drop Factor

		kMaxHandling
	};

	void changeKeybind(Keybind option, sf::Keyboard::Key newValue);
	void changeHandling(Handling option, HandlingType newValue);
	sf::Keyboard::Key getKeybind(Keybind option);
	HandlingType getHandling(Handling option);


private:
	std::vector<sf::Keyboard::Key> keybind;
	std::vector<HandlingType> handling;

public: 
	Settings(){
		keybind.resize(kMaxKeybind);
		handling.resize(kMaxHandling);
	}
};

static Settings settings;

#endif
