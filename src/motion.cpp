#include "config.hpp"
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <utility>
#include <vector>
#include "player.hpp"
#include "debug.hpp"


void Player::motion_register(Keybind t, int s){
    if(s == 1){
        if(t == Keybind::move_left) move_left(), hold_time[(int)Keybind::move_right].second.restart();
        if(t == Keybind::soft_drop) move_down();
        if(t == Keybind::move_right) move_right(), hold_time[(int)Keybind::move_left].second.restart();
    }
    hold_time[(int)t].first = s;
    if(s == 1) hold_time[(int)t].second.restart();
}

void Player::do_motion(){ // there's actually no ARR and SDD support yet :(
    if(hold_time[(int)Keybind::soft_drop].first == 1 && hold_time[(int)Keybind::soft_drop].second.getElapsedTime() > get_timing(Timing::SDD)){
        if(get_timing(Timing::ARR) == sf::Time(sf::milliseconds(0))){
            while(move_down());
        }
    } 
    if(hold_time[(int)Keybind::move_left].first == 1 && hold_time[(int)Keybind::move_left].second.getElapsedTime() > get_timing(Timing::DAS)){
        if(get_timing(Timing::ARR) == sf::Time(sf::milliseconds(0))){
            while(move_left());
        }
    } else if(hold_time[(int)Keybind::move_right].first == 1 && hold_time[(int)Keybind::move_right].second.getElapsedTime() > get_timing(Timing::DAS)){
        if(get_timing(Timing::ARR) == sf::Time(sf::milliseconds(0))){
            while(move_right());
        }
    }
}

void Player::event_handler(sf::Event event){
    if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased){
        int status = event.type == sf::Event::KeyPressed ? 1 : -1;
        for (Keybind motion : {Keybind::move_left, Keybind::move_right, Keybind::soft_drop})
        if(event.key.code == get_keybind(motion)) motion_register(motion, status);
    }
    if (event.type == sf::Event::KeyPressed){
        if(event.key.code == get_keybind(Keybind::hard_drop))  hard_drop();
        if(event.key.code == get_keybind(Keybind::rot_ccw))    rot_ccw();
        if(event.key.code == get_keybind(Keybind::rot_180))    rot_180();
        if(event.key.code == get_keybind(Keybind::rot_cw))     rot_cw();
        if(event.key.code == get_keybind(Keybind::swap))       swap();
        if(event.key.code == get_keybind(Keybind::reset))      reset();
        if(event.key.code == get_keybind(Keybind::undo))       undo();
    }

}
