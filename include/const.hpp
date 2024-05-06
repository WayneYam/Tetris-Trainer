#ifndef CONST_H
#define CONST_H

// board
#include <SFML/Graphics.hpp>
#include <vector>

const int row_number = 20;
const int column_number = 10;
const int cell_size = 30;
const sf::Vector2f upper_left_position(100, 100);
const sf::Color gridline_color (255, 255, 255);

const std::vector<sf::Color> color_list {
    sf::Color(255, 0, 0),
    sf::Color(255, 128, 0),
    sf::Color(255, 255, 0),
    sf::Color(0, 255, 0),
    sf::Color(0, 255, 255),
    sf::Color(0, 0, 255),
    sf::Color(127, 0, 255),
    sf::Color(64, 64, 64)
};

#endif
