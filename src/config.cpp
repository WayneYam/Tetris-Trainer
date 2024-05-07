#include "config.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

std::vector<sf::Keyboard::Key> motion = {
    sf::Keyboard::A, // move left
    sf::Keyboard::S, // soft drop
    sf::Keyboard::D, // move right
};

std::vector<std::string> motion_description = {
    "Move left",
    "Soft drop",
    "Move right",
};

std::vector<sf::Keyboard::Key> keybind = {
    sf::Keyboard::Space, // hard drop
    sf::Keyboard::J, // ccw
    sf::Keyboard::K, // 180
    sf::Keyboard::L, // cw
    sf::Keyboard::LShift, // hold
    sf::Keyboard::F4, // restart
    sf::Keyboard::F2, // config
    sf::Keyboard::Z,
};

std::vector<std::string> keybind_description = {
    "Hard drop",
    "Rotate CCW",
    "Rotate 180",
    "Rotate CW",
    "Swap hold piece",
    "Restart",
    "Config (not implemented)",
    "Undo (Ctrl is not read)",
};

std::vector<sf::Time> timing = {
    sf::Time(sf::milliseconds(133)), // DAS
    sf::Time(sf::milliseconds(0)),   // ARR
    sf::Time(sf::milliseconds(0)),   // SDD, inverse of SDF
};

std::vector<std::string> timing_description = {
    "DAS(ms)",
    "ARR(ms) (not yet implemented)",
    "SDD(ms), inverse of SDF(not yet implemented)",
};

std::vector<int> option {
    1, // ghost
};

std::vector<std::string> option_description = {
    "Ghost piece? (1 for yes, 0 for no)",
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

sf::Keyboard::Key get_key_from_user(std::string s, int idx, sf::RenderWindow &window){
    std::cout << "Press key for: (";
    if(s == "motion") {
        std::cout << motion_description[idx];
    }else if(s == "keybind"){
        std::cout << keybind_description[idx];
    }
    std::cout << ")" << std::endl;
    
    sf::Event event;
    while(window.waitEvent(event)){
        if(event.type == sf::Event::KeyPressed){
            std::cout << event.key.code << std::endl;
            return event.key.code;
        }
    }
    return sf::Keyboard::Unknown;
}

int get_int_from_user(std::string s, int idx){
    std::cout << "input integer for: (";
    if(s == "timing") {
        std::cout << timing_description[idx];
    }else if(s == "option"){
        std::cout << option_description[idx];
    }else{
        std::cout << "Unknown" << '\n';
    }
    std::cout << ")" << std::endl;
    
    int x;
    std::cin >> x;
    std::cout << x << std::endl;
    return x;
}

void read_config_from_user(sf::RenderWindow &window){
    for(int i = 0; i < (int)motion.size(); i++) motion[i] = get_key_from_user("motion", i, window);
    for(int i = 0; i < (int)keybind.size(); i++) keybind[i] = get_key_from_user("keybind", i, window);
    for(int i = 0; i < (int)timing.size(); i++) timing[i] = sf::milliseconds(get_int_from_user("timing", i));
    for(int i = 0; i < (int)option.size(); i++) option[i] = get_int_from_user("option", i);

    write_config();
}

bool read_config(){
    std::ifstream myfile("config/config.txt");

    for(int i = 0; i < (int)motion.size(); i++){
        int x;
        if(!myfile.good()) return 0;
        myfile >> x;
        motion[i] = (sf::Keyboard::Key)x;
    }
    for(int i = 0; i < (int)keybind.size(); i++){
        int x;
        if(!myfile.good()) return 0;
        myfile >> x;
        keybind[i] = (sf::Keyboard::Key)x;
    }
    for(int i = 0; i < (int)timing.size(); i++){
        int x;
        if(!myfile.good()) return 0;
        myfile >> x;
        timing[i] = sf::milliseconds(x);
    }
    for(int i = 0; i < (int)option.size(); i++){
        if(!myfile.good()) return 0;
        myfile >> option[i];
    }
    myfile.close();

    return 1;
}
