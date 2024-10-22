#ifndef CONFIG_H
#define CONFIG_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Keyboard.hpp>

enum class Keybind {
    move_left,
    soft_drop,
    move_right,
    hard_drop,
    rot_ccw,
    rot_180,
    rot_cw,
    swap,
    reset,
    config,
    undo,
};

enum class Timing{
    DAS,
    ARR,
    SDD,
};

enum class Option{
    ghost,
};

sf::Keyboard::Key get_keybind(Keybind k);
sf::Time get_timing(Timing t);
int get_option(Option o);

void set_keybind(Keybind k, sf::Keyboard::Key K);
void set_timing(Timing t, sf::Time T);
void set_option(Option o, int x);

void write_config();
bool read_config();
void read_config_from_user(sf::RenderWindow &window);

namespace config{
const int motion_num = 3; // only the first three do we care about long press
}


#endif
