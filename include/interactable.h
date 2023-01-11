#ifndef INTERACTABLE_H
#define INTERACTABLE_H

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <functional>

class Interactable : public sf::Sprite {
	
public:
	enum State{
		kNull,		// Not interacting
		kHover,		// Hovering
		kPress,		// Pressing
		kMaxState
	};

	typedef std::function<void()> Action;
	const static inline Action nullAction{[](){}};

private:
	bool active{false};
	int priority{0};
	State state{kNull};	

public:
	Action onHover{nullAction};		// When mouse hovers over
	Action onExit{nullAction};		// When mouse is no longer hovering
	Action onClick{nullAction};		// When object is clicked
	Action onPress{nullAction};  	// While object is pressed
	Action onRelease{nullAction};	// When mouse button is released

	Interactable(const sf::Texture& texture, const sf::IntRect& rectangle, int pri, Action hover, Action exits, Action click, Action press, Action released) 
	: sf::Sprite(texture, rectangle), priority(pri), onHover(hover), onExit(exits), onClick(click), onPress(press), onRelease(released) {}
	Interactable(const sf::Texture& texture, const sf::IntRect& rectangle, int pri) 
	: Interactable(texture, rectangle, pri, nullAction, nullAction, nullAction, nullAction, nullAction) {}
	Interactable(const sf::Texture& texture, int pri) : Interactable(texture, sf::IntRect(), pri) {}
	Interactable(int pri) : Interactable(sf::Texture() ,pri) {}
	Interactable() : Interactable(0) {}


	bool inBound(sf::Vector2f v); 	// Checks whether a position is in bound
};

#endif


