#ifndef CONFIG_H
#define CONFIG_H

#include <SFML/System/Time.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <vector>

namespace config{

const std::vector<sf::Keyboard::Key> motion = {
    sf::Keyboard::A, // move left
    sf::Keyboard::S, // soft drop
    sf::Keyboard::D, // move right
};

const std::vector<sf::Keyboard::Key> keybind = {
    sf::Keyboard::Space, // hard drop
    sf::Keyboard::J, // ccw
    sf::Keyboard::K, // 180
    sf::Keyboard::L, // cw
    sf::Keyboard::LShift, // hold
    sf::Keyboard::F4, // restart
};

const auto DAS = sf::Time(sf::milliseconds(133));
const auto ARR = sf::Time(sf::milliseconds(0));
const auto SDD = sf::Time(sf::milliseconds(0)); // soft drop delay, inverse of SDF

const bool ghost = 1; // draw ghost piece

}



#endif
