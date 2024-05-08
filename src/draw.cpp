#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>

#include "draw.hpp"
#include "const.hpp"
#include "types.hpp"
#include "queue.hpp"
#include "config.hpp"
#include "board.hpp"

const std::vector<sf::Color> color_list {
    sf::Color(255, 0, 0)  , // Z
    sf::Color(255, 128, 0), // L
    sf::Color(255, 255, 0), // O
    sf::Color(0, 255, 0)  , // S
    sf::Color(0, 255, 255), // I
    sf::Color(0, 0, 255)  , // J
    sf::Color(127, 0, 255), // T
    sf::Color(64, 64, 64)   // garbage
};

const int N = row_number, M = column_number; // board row/column
const int sz = 30;
const sf::Vector2f pos(150, 150);
const sf::Color grid(64, 64, 64);



sf::Color get_color(int c){
    if(0 <= c && c < (int)color_list.size()) return color_list[c];
    return sf::Color(0, 0, 0);
}

void draw_board(sf::RenderWindow &window){
    Board board = getBoard();

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
            if(get_option(Option::ghost) && board[i+3][j] >= 10){
                cur_color = get_color(board[i+3][j] - 10);
                cur_color.a = 64;
            }
            cur.setFillColor(cur_color);
            cur.setPosition(pos.x + j*sz + 1, pos.y + i*sz + 1);
            window.draw(cur);
        }
    }

}

void draw_piece(sf::RenderWindow &window, int t, sf::Vector2f pos, float size){
    // Z L O S I J T

    float offx;
    float offy = size / 4;
    if(t == 1 || t == 5 || t == 0 || t == 3 || t == 6){
        offx = size / 8; 
    } else{
        offx = 0;
    }

    pos += sf::Vector2f(offx, offy);

    for(auto [x, y] : get_dis(t, 0)){
        sf::RectangleShape cur (sf::Vector2f(size/4 - 2, size/4 - 2));
        sf::Color cur_color = get_color(t);
        cur.setFillColor(cur_color);
        cur.setPosition(pos.x + y*size/4 + 1, pos.y + x*size/4 + 1);
        window.draw(cur);
    }
}

void draw_queue(sf::RenderWindow &window){

    sf::Vector2f queue_position(475, 150);
    float piece_size = 80;

    std::deque<int> queue = get_queue();
    for(int i = 0; i < 5; i++){
        auto current_position = queue_position;
        current_position.y += i * piece_size;
        draw_piece(window, queue[i], current_position, piece_size);
    }
}

void draw_hold_piece(sf::RenderWindow &window){

    sf::Vector2f hold_position(50, 400);
    float piece_size = 80;

    int t = get_hold_piece();
    if(t != -1) draw_piece(window, t, hold_position, piece_size);
}

void draw_data(sf::RenderWindow &window){
    sf::Font font;
    font.loadFromFile("resources/arial.ttf");
    sf::Text text("total spike", font, 30);
    window.draw(text);
}
