#ifndef DRAW_H
#define DRAW_H

#include <SFML/Graphics.hpp>
#include "board.hpp"

void draw_board(sf::RenderWindow &window, Board B);
void draw_queue(sf::RenderWindow &window, Queue Q);
void draw_hold_piece(sf::RenderWindow &window, int t);
void draw_data(sf::RenderWindow &window);
#endif
