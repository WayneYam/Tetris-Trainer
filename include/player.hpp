#ifndef PLAYER_H
#define PLAYER_H

#include <ctime>
#include <random>
#include "board.hpp"

struct Player{
    std::uniform_real_distribution<float> real_distribution;
    std::uniform_int_distribution<int> int_distribution;
    Board B;
	float time, app, apm;
	int piece, spike, spike_pos, line;
	int combo, btb_count;
	int garbage_line[100];
	float garbage_chaos=0.4;
	int index_start, index_end;
	bool is_tspin;

	Player(int n, int m);
	int spike_count();
	void garbage_gen();
	void update();
    Board getBoard();

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
