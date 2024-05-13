#ifndef AI_H
#define AI_H

#include "player.hpp"

struct PlayerAI : public Player {
    PlayerAI(int n, int m);
    void event_handler(sf::Event e) override;
    void reset() override;
    void hard_drop() override;
    void do_motion() override;
    void place_a_piece();
};

#endif
