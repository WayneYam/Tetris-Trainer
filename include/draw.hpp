#ifndef DRAW_H
#define DRAW_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "const.hpp"
#include "types.hpp"

void draw_board(sf::RenderWindow &window);
void draw_queue(sf::RenderWindow &window);
void draw_hold_piece(sf::RenderWindow &window);

#endif
