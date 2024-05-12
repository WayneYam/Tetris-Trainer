#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <ctime>
#include <queue>
#include <random>
#include "board.hpp"
#include "config.hpp"

struct Player{
    std::uniform_real_distribution<float> real_distribution;
    std::uniform_int_distribution<int> int_distribution;
    std::default_random_engine generator;
    Board B;
    sf::Clock clock;
	int piece_count, attack_count, spike_pos;
	int combo, btb_count;
	float garbage_chaos=0.4;

	Player(int n, int m);

    std::queue<int> garbage;
	virtual int spike_count(Lineclear l);
	virtual void garbage_gen();
    Board getBoard();

    std::vector<std::pair<int, sf::Clock> > hold_time;
    virtual void event_handler(sf::Event e);
    virtual void motion_register(Keybind k, int status);
    virtual void do_motion();

    virtual bool move_left();
    virtual bool move_down();
    virtual bool move_right();
    virtual void hard_drop();
    virtual bool rot_ccw();
    virtual bool rot_180();
    virtual bool rot_cw();
    virtual void reset();
    virtual void swap();
    virtual void undo();
};




#endif
