#include <SFML/Graphics.hpp>
#include "draw.hpp"
#include "board.hpp"
#include "config.hpp"
#include "motion.hpp"
#include "debug.hpp"
#include "player.hpp"

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

    Player P(20, 10);

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
                    if(event.key.code == get_keybind(motion)) motion_register(motion, status, P.B);
            }
            if (event.type == sf::Event::KeyPressed){
                if(event.key.code == get_keybind(Keybind::hard_drop))  P.hard_drop();
                if(event.key.code == get_keybind(Keybind::rot_ccw))    P.rot_ccw();
                if(event.key.code == get_keybind(Keybind::rot_180))    P.rot_180();
                if(event.key.code == get_keybind(Keybind::rot_cw))     P.rot_cw();
                if(event.key.code == get_keybind(Keybind::swap))       P.swap();
                if(event.key.code == get_keybind(Keybind::reset))      P.reset();
                if(event.key.code == get_keybind(Keybind::undo))       P.undo();
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

        do_motion(P.B);

        window.clear();
        draw_board(window, P.B);
        draw_queue(window, P.B.queue);
        draw_hold_piece(window, P.B.get_hold_piece());
        // draw_data(window);

        window.display();
    }

    write_config();

    return 0;
}
