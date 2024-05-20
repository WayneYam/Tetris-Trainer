#include "player.hpp"

#include <algorithm>
#include <ctime>
#include <random>

#include "board.hpp"
#include "config.hpp"
#include "debug.hpp"

// for garbage line generate

Player::Player(int n, int m)
    : real_distribution(0.0, 1.0), int_distribution(0, m - 1), generator(time(NULL)), B(n, m), hold_time(config::motion_num){
    piece_count = attack_count =
        0;
    combo = btb_count = -1;
    spike_pos = int_distribution(generator);
    clock.restart();
    B.init_board();
}

int Player::spike_count(Lineclear l) {
    return l.lines;
    if (l.lines == 0) {
        combo = -1;
        return 0 + l.is_pc * 10;
    }
    combo++;
    int basic, btb;
    // atk=(basic+b2b)(1+combo*0.25)
    if (l.tspin) {
        basic = 2 * l.lines;
        btb_count++;
    } else if (l.lines == 4) {
        basic = 4;
        btb_count++;
    } else {
        basic = l.lines - 1;
        btb_count = -1;
    }

    if (btb_count < 1)
        btb = 0;
    else if (btb_count < 3)
        btb = 1;
    else if (btb_count < 8)
        btb = 2;
    else if (btb_count < 24)
        btb = 3;
    else if (btb_count < 67)
        btb = 4;
    else
        btb = 5;
    if (basic + btb == 0) {
        if (combo < 2) return 0 + l.is_pc * 10;
        if (combo < 6) return 1 + l.is_pc * 10;
        if (combo < 16) return 2 + l.is_pc * 10;
        return 3 + l.is_pc * 10;
    }
    return std::max((basic + btb) * (4 + combo) / 4 + l.is_pc * 10, 0);
}
void Player::garbage_gen() {
    while (!garbage.empty()) {
        float x = real_distribution(generator);
        int n = garbage.front();
        garbage.pop();
        if (x > garbage_chaos) {
            // spike_pos = spike_pos;
        } else {
            spike_pos = int_distribution(generator);
        }
        for (int k = 2 * B.N - n - 1; k >= 0; k--) {
            B[k + n] = B[k];
        }
        for (int k = 0; k < B.M; k++) {
            for (int j = 0; j < n; j++) {
                if (k != spike_pos)
                    B[j][k] = 7;
                else
                    B[j][k] = -1;
            }
        }
    }
}

Board Player::getBoard() { return B; }
bool Player::move_left() {return B.move_piece(2);}
bool Player::move_down() {return B.move_piece(3);}
bool Player::move_right() {return B.move_piece(0);}
bool Player::rot_ccw() {return B.rotate_piece(3);}
bool Player::rot_180() {return B.rotate_piece(2);}
bool Player::rot_cw() {return B.rotate_piece(1);}

void Player::hard_drop(){
    auto ret = B.hard_drop();
    piece_count++;
    int dam = spike_count(ret);
    // cancellation
    if(dam){
        while(!garbage.empty() && dam >= garbage.front() ){
            dam -= garbage.front();
            garbage.pop();
        }
        if(!garbage.empty()) garbage.front() -= dam;
    }
    attack_count += dam;
    if(ret.lines == 0){
        garbage_gen();
    }
    if(B.init_piece(B.queue.get_piece())) reset();
}

void Player::swap(){
    B.swap_piece();
}

void Player::undo(){
    B.undo();
}

void Player::reset(){
    B.reset();
    piece_count = attack_count = 0;
    combo = btb_count = -1;
    clock.restart();
    while(garbage.size()) garbage.pop();
}


