#include <SFML/Graphics.hpp>
#include "draw.hpp"
#include "config.hpp"
#include "debug.hpp"
#include "player.hpp"
#include "mode.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(600, 800), "Tetris Trainer");
    window.setKeyRepeatEnabled(false);
    window.setFramerateLimit(240);
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
            P.event_handler(event);
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::Resized) {
                view.setSize({
                    static_cast<float>(event.size.width),
                    static_cast<float>(event.size.height)
                });
                window.setView(view);
            }
        }
        P.do_motion();
        window.clear();
        draw_player(window, P, {50, 750}, 1);
        window.display();
    }

    write_config();

    return 0;
}
