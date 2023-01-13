#ifndef CHECKBOX_H
#define CHECKBOX_H

#include "interactable.h"

class Checkbox : public Interactable {
	private:
		bool ticked{false};
	public:

		Checkbox(
			  const Action &hover = nullAction
			, const Action &exits = nullAction
			, const Action &click = nullAction
			, const Action &press = nullAction
			, const Action &released = nullAction
			, const int &pri = 0
			, const sf::Texture& texture = sf::Texture()
			, const sf::IntRect& rectangle = sf::IntRect()) 

			: Interactable(hover, exits, click, press, released, pri, texture, rectangle), ticked{false} {}

		void tick();
};

#endif
