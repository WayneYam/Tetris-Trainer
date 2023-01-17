#ifndef INTERACTABLE_MANAGER_H
#define INTERACTABLE_MANAGER_H

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <functional>
#include <vector>
#include <memory>

class InteractableManager{

public:

	class Interactable;
	using ObjCollection = std::vector<std::shared_ptr<Interactable> >;

private:
	
	ObjCollection objs;

public:
	
	void addObj(std::shared_ptr<Interactable> obj){
		objs.resize(objs.size() + 1);
		objs[objs.size() - 1] = obj;
	}

public: 
	class Interactable : public sf::Sprite {
		friend class InteractableManager;
		public:
			enum State{
				kNull,		// Not interacting
				kHover,		// Hovering
				kPress,		// Pressing
				kMaxState
			};

			typedef std::function<void(Interactable* )> Action;
			const static inline Action nullAction{[](Interactable* ){}};

		private:
			bool active{true}; // Whether this object is interactable
			int priority{0};    // Decides which object activates when overlapping
			State state{kNull};	

		public:
			Action onHover{nullAction};		// When mouse hovers over
			Action onLeave{nullAction};		// When mouse is no longer hovering
			Action onClick{nullAction};		// When object is clicked
			Action onPress{nullAction};  	// While object is pressed
			Action onRelease{nullAction};	// When mouse button is released

			void hover(){onHover(this);}
			void leave(){onLeave(this);}
			void press(){onPress(this);}
			void click(){onClick(this);}
			void release(){onRelease(this);}

			Interactable(Action hover = nullAction
					, Action exits = nullAction
					, Action click = nullAction
					, Action press = nullAction
					, Action released = nullAction
					, int pri = 0
					, const sf::Texture& texture = sf::Texture()
					, const sf::IntRect& rectangle = sf::IntRect()) 
				: sf::Sprite(texture, rectangle), active{true}, priority{pri}, state{kNull}, onHover(hover) , onLeave(exits) , onClick(click) , onPress(press) , onRelease(released) {  }

			bool inBound(sf::Vector2f v); 	// Checks whether a position is in bound
	};


	std::shared_ptr<InteractableManager::Interactable> query(sf::Vector2f position);
	void update(sf::Vector2f v);
};


#endif
