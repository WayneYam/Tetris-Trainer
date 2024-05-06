#include "motion.hpp"
#include "config.hpp"
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <utility>
#include <vector>
#include "board.hpp"

std::vector<std::pair<int, sf::Clock> > status(config::motion_num);

void motion_register(Keybind t, int s){
    if(s == 1){
        if(t == Keybind::move_left) move_piece(2), status[2].second.restart();
        if(t == Keybind::soft_drop) move_piece(3);
        if(t == Keybind::move_right) move_piece(0), status[0].second.restart();
    }
    status[(int)t].first = s;
    if(s == 1) status[(int)t].second.restart();
}

void do_motion(){
    if(status[1].first == 1 && status[1].second.getElapsedTime() > get_timing(Timing::SDD)){
        if(get_timing(Timing::ARR) == sf::Time(sf::milliseconds(0))){
            while(move_piece(3));
        }
    } 
    if(status[0].first == 1 && status[0].second.getElapsedTime() > get_timing(Timing::DAS)){
        if(get_timing(Timing::ARR) == sf::Time(sf::milliseconds(0))){
            while(move_piece(2));
        }
    } else if(status[2].first == 1 && status[2].second.getElapsedTime() > get_timing(Timing::DAS)){
        if(get_timing(Timing::ARR) == sf::Time(sf::milliseconds(0))){
            while(move_piece(0));
        }
    }
}
