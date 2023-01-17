#ifndef ELEMENT_H
#define ELEMENT_H

#include <vector>
#include <memory>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

class Element : public sf::Drawable{

private:
	
	sf::Vector2f pos{0, 0};
	Element* parent {nullptr};
	std::vector<std::shared_ptr<Element> > child{};
	bool isLeaf{true};
	std::shared_ptr<sf::Drawable> unit{nullptr};

public:
	
	Element(){};
	Element(std::shared_ptr<sf::Drawable> ptr){
		unit = ptr;
	}

	void draw(sf::RenderTarget &target, sf::RenderStates states = sf::RenderStates() ) const ;
	
	void assignUnit(std::shared_ptr<sf::Drawable> childptr);
	void addChild(std::shared_ptr<Element> child);
	void addChild(std::shared_ptr<sf::Drawable> child);

	void translate(sf::Vector2f v){
		pos += v;
	}
};

#endif
