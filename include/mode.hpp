#ifndef MODE_H
#define MODE_H

#include "player.hpp"

struct PlayerBackfire : public Player{
    PlayerBackfire(int n, int m) : Player(n, m){}
    void hard_drop();
};

struct PlayerCheese : public Player{
    PlayerCheese (int n, int m);
    int max_layer = 9, min_layer = 3;
    void hard_drop();
    void garbage_gen();
    void reset();
    int current_cheese_layer();
    void gen_cheese_to(int n);
};

#endif
