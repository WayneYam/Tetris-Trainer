#ifndef DRAW_H
#define DRAW_H

#include <SFML/Graphics.hpp>
#include "player.hpp"

void draw_player(sf::RenderWindow &window, Player &P, sf::Vector2f pos, float sz = 1);
#endif
