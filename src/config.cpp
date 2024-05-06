#include "config.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <vector>

std::vector<sf::Keyboard::Key> motion = {
    sf::Keyboard::A, // move left
    sf::Keyboard::S, // soft drop
    sf::Keyboard::D, // move right
};

std::vector<sf::Keyboard::Key> keybind = {
    sf::Keyboard::Space, // hard drop
    sf::Keyboard::J, // ccw
    sf::Keyboard::K, // 180
    sf::Keyboard::L, // cw
    sf::Keyboard::LShift, // hold
    sf::Keyboard::F4, // restart
};

std::vector<sf::Time> timing = {
    sf::Time(sf::milliseconds(133)), // DAS
    sf::Time(sf::milliseconds(0)),   // ARR
    sf::Time(sf::milliseconds(0)),   // SDD, inverse of SDF
};

std::vector<int> option {
    1, // ghost
};

sf::Keyboard::Key get_keybind(Keybind k){
    if((int)k < 3) return motion[(int)k];
    else return keybind[(int)k-3];
}


sf::Time get_timing(Timing t) { return timing[(int)t];};
int get_option(Option o) {return option[(int)o];}
