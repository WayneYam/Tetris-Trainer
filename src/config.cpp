#include "config.hpp"
#include <SFML/System/Time.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <vector>
#include <iostream>
#include <fstream>

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
    sf::Keyboard::F2, // config
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

void write_config(){
    std::ofstream myfile;
    myfile.open ("config/config.txt");
    for(auto x : motion) myfile << (int)x << ' '; 
    myfile << '\n';
    for(auto x : keybind) myfile << (int)x << ' '; 
    myfile << '\n';
    for(auto x : timing) myfile << x.asMilliseconds() << ' ';
    myfile << '\n';
    for(auto x : option) myfile << x << ' ';
    myfile << '\n';
    myfile.close();
}

void read_config(){
    std::ifstream myfile("config/config.txt");

    for(int i = 0; i < (int)motion.size(); i++){
        int x;
        myfile >> x;
        motion[i] = (sf::Keyboard::Key)x;
    }
    for(int i = 0; i < (int)keybind.size(); i++){
        int x;
        myfile >> x;
        keybind[i] = (sf::Keyboard::Key)x;
    }
    for(int i = 0; i < (int)timing.size(); i++){
        int x;
        myfile >> x;
        timing[i] = sf::milliseconds(x);
    }
    for(int i = 0; i < (int)option.size(); i++){
        myfile >> option[i];
    }
    myfile.close();
}
