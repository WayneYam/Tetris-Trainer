#include "element.h"

void Element::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	states.transform.translate(this->pos);
	if(unit){
		target.draw(*unit, states);	
	}
	for(int i = 0; i < (int)child.size(); i++){
		child[i]->draw(target, states);
	}
}

void Element::assignUnit(std::shared_ptr<sf::Drawable> childptr){
	this->unit = std::move(childptr);
}

void Element::addChild(std::shared_ptr<Element> child){
	this->child.resize(this->child.size() + 1);
	this->child[this->child.size() - 1] = std::move(child);
	this->parent = this;
}

void Element::addChild(std::shared_ptr<sf::Drawable> childptr){
	this->child.resize(this->child.size() + 1);
	this->child.back() = std::make_shared<Element>();
	this->child.back()->assignUnit(std::move(childptr));
	this->parent = this;
}
