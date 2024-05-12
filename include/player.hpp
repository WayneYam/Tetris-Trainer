#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Window/Event.hpp>
#include <ctime>
#include <queue>
#include <random>
#include "board.hpp"
#include "config.hpp"

struct Player{
    std::uniform_real_distribution<float> real_distribution;
    std::uniform_int_distribution<int> int_distribution;
    Board B;
	float time, app, apm;
	int piece, spike, spike_pos;
	int combo, btb_count;
	float garbage_chaos=0.4;

	Player(int n, int m);

    std::queue<int> garbage;
	int spike_count(Lineclear l);
	void garbage_gen();
	void update(Lineclear l);
    Board getBoard();

    std::vector<std::pair<int, sf::Clock> > hold_time;
    void event_handler(sf::Event e);
    void motion_register(Keybind k, int status);
    void do_motion();

    bool move_left();
    bool move_down();
    bool move_right();
    void hard_drop();
    bool rot_ccw();
    bool rot_180();
    bool rot_cw();
    void reset();
    void swap();
    void undo();
};




#endif
