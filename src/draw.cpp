#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>

#include "draw.hpp"
#include "const.hpp"
#include "types.hpp"
#include "queue.hpp"

const int N = row_number, M = column_number; // board row/column
const int sz = cell_size;
const sf::Vector2f pos = upper_left_position;
const sf::Color grid = gridline_color;


sf::Color get_color(int c){
    if(0 <= c && c < (int)color_list.size()) return color_list[c];
    return sf::Color(0, 0, 0);
}

void draw_board(sf::RenderWindow &window, const Board &board){

    // draw row line
    for(int i = 0; i <= N; i++){
        sf::VertexArray lines(sf::LinesStrip, 2);
        lines[0].position = sf::Vector2f(pos.x, pos.y + i * sz);
        lines[1].position = sf::Vector2f(pos.x + M * sz, pos.y + i * sz);
        lines[0].color = lines[1].color = grid;
        window.draw(lines);
    }

    // draw column line
    for(int i = 0; i <= M; i++){
        sf::VertexArray lines(sf::LinesStrip, 2);
        lines[0].position = sf::Vector2f(pos.x + i * sz, pos.y);
        lines[1].position = sf::Vector2f(pos.x + i * sz, pos.y + N * sz);
        lines[0].color = lines[1].color = grid;
        window.draw(lines);
    }

    for(int i = -3; i < N; i++){
        for(int j = 0; j < M; j++){
            sf::RectangleShape cur (sf::Vector2f(sz - 2, sz - 2));
            sf::Color cur_color = get_color(board[i+3][j]);
            cur.setFillColor(cur_color);
            cur.setPosition(pos.x + j*sz + 1, pos.y + i*sz + 1);
            window.draw(cur);
        }
    }
}


