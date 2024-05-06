#include "motion.hpp"
#include "config.hpp"
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <utility>
#include <vector>
#include "board.hpp"

std::vector<std::pair<int, sf::Clock> > status(config::motion.size());

void motion_register(int t, int s){
    if(s == 1){
        if(t == 0) move_piece(2), status[2].second.restart();
        if(t == 1) move_piece(3);
        if(t == 2) move_piece(0), status[0].second.restart();
    }
    status[t].first = s;
    if(s == 1) status[t].second.restart();
}

void do_motion(){
    if(status[1].first == 1 && status[1].second.getElapsedTime() > config::SDD){
        if(config::ARR == sf::Time(sf::milliseconds(0))){
            while(move_piece(3));
        }
    } 
    if(status[0].first == 1 && status[0].second.getElapsedTime() > config::DAS){
        if(config::ARR == sf::Time(sf::milliseconds(0))){
            while(move_piece(2));
        }
    } else if(status[2].first == 1 && status[2].second.getElapsedTime() > config::DAS){
        if(config::ARR == sf::Time(sf::milliseconds(0))){
            while(move_piece(0));
        }
    }
}
