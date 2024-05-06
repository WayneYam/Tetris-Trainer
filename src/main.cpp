#include <SFML/Graphics.hpp>
#include "draw.hpp"
#include "board.hpp"
#include "config.hpp"
#include "motion.hpp"
#include "queue.hpp"

void reset(){
    reset_board();
    reset_queue(); 
    init_board();
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(600, 800), "SFML works!");
    window.setKeyRepeatEnabled(false);

	// sf::Texture texture;
	// texture.loadFromFile("resources/blockskin.png");

    init_board();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased){
                for(int i = 0; i < (int)config::motion.size(); i++){
                    if(event.key.code == config::motion[i]) {
                        motion_register(i, (event.type == sf::Event::KeyPressed ? 1 : -1)); 
                    }
                }
            }
            if (event.type == sf::Event::KeyPressed){
                if(event.key.code == config::keybind[0]) hard_drop();
                if(event.key.code == config::keybind[1]) rotate_piece(3);
                if(event.key.code == config::keybind[2]) rotate_piece(2);
                if(event.key.code == config::keybind[3]) rotate_piece(1);
                if(event.key.code == config::keybind[4]) swap_piece();
                if(event.key.code == config::keybind[5]) reset();
            }
        }

        do_motion();

        window.clear();
        draw_board(window);
        draw_queue(window);
        draw_hold_piece(window);

        window.display();
    }

    return 0;
}
