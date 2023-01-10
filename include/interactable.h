#ifndef INTERACTABLE_H
#define INTERACTABLE_H

#include <SFML/Graphics/Sprite.hpp>
#include <functional>

const static inline std::function<void()> nullAction{[](){}};
class Interactable : public sf::Sprite {
	
public:
	enum State{
		kNull,		// Not interacting
		kHover,		// Hovering
		kPress,		// Pressing
		kMaxState
	};

	typedef std::function<void()> Action;

private:
	bool active{false};
	int priority{0};
	State state{kNull};	

	Action onHover{nullAction};		// When mouse hovers over
	Action onExit{nullAction};		// When mouse is no longer hovering
	Action onClick{nullAction};		// When object is clicked
	Action onPressed{nullAction};	// While object is pressed
	Action onRelease{nullAction};	// When mouse button is released

public:



	Interactable(int pri) : sf::Sprite{}, priority{pri} {}
	Interactable(const sf::Texture& texture, int pri) : sf::Sprite{texture}, priority{pri} {}
	Interactable(const sf::Texture& texture, const sf::IntRect& rectangle, int pri) : sf::Sprite(texture, rectangle), priority{pri}  {}


	bool inBound(sf::Vector2f v); 	// Checks whether a position is in bound
};

#endif


