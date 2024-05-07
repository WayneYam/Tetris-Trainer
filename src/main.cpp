#include <SFML/Graphics.hpp>
#include "draw.hpp"
#include "board.hpp"
#include "config.hpp"
#include "motion.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(600, 800), "Tetris Trainer");
    window.setKeyRepeatEnabled(false);
    sf::View view = window.getDefaultView();

	// sf::Texture texture;
	// texture.loadFromFile("resources/blockskin.png");

    if(!read_config()){
        read_config_from_user(window);
    }

    init_board();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased){
                int status = event.type == sf::Event::KeyPressed ? 1 : -1;
                for (Keybind motion : {Keybind::move_left, Keybind::move_right, Keybind::soft_drop})
                    if(event.key.code == get_keybind(motion)) motion_register(motion, status);
            }
            if (event.type == sf::Event::KeyPressed){
                if(event.key.code == get_keybind(Keybind::hard_drop)) hard_drop();
                if(event.key.code == get_keybind(Keybind::rot_ccw)) rotate_piece(3);
                if(event.key.code == get_keybind(Keybind::rot_180)) rotate_piece(2);
                if(event.key.code == get_keybind(Keybind::rot_cw)) rotate_piece(1);
                if(event.key.code == get_keybind(Keybind::swap)) swap_piece();
                if(event.key.code == get_keybind(Keybind::reset)) reset();
                if(event.key.code == get_keybind(Keybind::undo)) undo();
            }
            if (event.type == sf::Event::Resized) {
                // resize my view
                view.setSize({
                    static_cast<float>(event.size.width),
                    static_cast<float>(event.size.height)
                });
                window.setView(view);
                // and align shape
            }
        }

        do_motion();

        window.clear();
        draw_board(window);
        draw_queue(window);
        draw_hold_piece(window);

        window.display();
    }

    write_config();

    return 0;
}
