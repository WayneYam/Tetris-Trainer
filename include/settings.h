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
	
	static int foo;

	static void Init();
	static void ChangeKeybind(Keybind option, sf::Keyboard::Key newValue);
	static void ChangeHandling(Handling option, HandlingType newValue);
	static sf::Keyboard::Key GetKeybind(Keybind option);
	static HandlingType GetHandling(Handling option);


private:
	Settings(){} // disables instantiation

	static std::vector<sf::Keyboard::Key> keybind;
	static std::vector<HandlingType> handling;
};

int Settings::foo{3};
std::vector<sf::Keyboard::Key> Settings::keybind = {};
std::vector<Settings::HandlingType> Settings::handling = {};

#endif
