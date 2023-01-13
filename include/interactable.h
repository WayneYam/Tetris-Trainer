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
	bool active{false}; // Whether this object is interactable
	int priority{0};    // Decides which object activates when overlapping
	State state{kNull};	

public:
	Action onHover{nullAction};		// When mouse hovers over
	Action onExit{nullAction};		// When mouse is no longer hovering
	Action onClick{nullAction};		// When object is clicked
	Action onPress{nullAction};  	// While object is pressed
	Action onRelease{nullAction};	// When mouse button is released

	Interactable(Action hover = nullAction
			   , Action exits = nullAction
			   , Action click = nullAction
			   , Action press = nullAction
			   , Action released = nullAction
			   , int pri = 0
			   , const sf::Texture& texture = sf::Texture()
			   , const sf::IntRect& rectangle = sf::IntRect()) 
			: sf::Sprite(texture, rectangle), active{false}, priority{pri}, state{kNull}, onHover(hover) , onExit(exits) , onClick(click) , onPress(press) , onRelease(released) {}

	bool inBound(sf::Vector2f v); 	// Checks whether a position is in bound
};

#endif


