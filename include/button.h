#ifndef BUTTON_H
#define BUTTON_H

#include "interactable.h"
#include <string>


class Button : public InteractableManager::Interactable {
	private:
		std::string caption;

	public:

		Button(
			  const std::string &text
			, const Action &hover = nullAction
			, const Action &leave = nullAction
			, const Action &click = nullAction
			, const Action &press = nullAction
			, const Action &released = nullAction
			, const int &pri = 0
			, const sf::Texture& texture = sf::Texture()
			, const sf::IntRect& rectangle = sf::IntRect()) 

			: InteractableManager::Interactable(hover, leave, click, press, released, pri, texture, rectangle), caption{text} {}
		
		void setCaption (const std::string &newCaption);
};

#endif
