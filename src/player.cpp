#include "player.hpp"

#include <ctime>
#include <random>

#include "board.hpp"

// for garbage line generate
int total_spike = 0;
std::default_random_engine generator(time(NULL));

Player::Player(int n, int m)
    : real_distribution(0.0, 1.0), int_distribution(0, m - 1), B(n, m) {
    total_spike = piece = spike = time = app = apm = index_start = index_end =
        0;
    combo = btb_count = -1;
    spike_pos = int_distribution(generator);
    B.init_board();
}
int Player::spike_count() {
    int l = line;
    line = 0;
    if (l == 0) {
        combo = -1;
        return 0;
    }
    combo++;
    int basic, btb;
    // atk=(basic+b2b)(1+combo*0.25)
    if (is_tspin) {
        basic = 2 * l;
        btb_count++;
    } else if (l == 4) {
        basic = 4;
        btb_count++;
    } else {
        basic = l - 1;
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
        if (combo < 2) return 0;
        if (combo < 6) return 1;
        if (combo < 16) return 2;
        return 3;
    }
    return (basic + btb) * (4 + combo) / 4;
}
void Player::garbage_gen() {
    while (index_start != index_end) {
        float x = real_distribution(generator);
        int n = garbage_line[index_start];
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
        index_start = (index_start + 1) % 100;
    }
}
void Player::update() {
    piece++;
    garbage_line[index_end] = spike_count();
    total_spike += garbage_line[index_end];
    index_end = (index_end + 1) % 100;
    garbage_gen();
}

Board Player::getBoard() { return B; }

bool Player::move_left() {return B.move_piece(2);}
bool Player::move_down() {return B.move_piece(3);}
bool Player::move_right() {return B.move_piece(0);}
bool Player::rot_ccw() {return B.rotate_piece(3);}
bool Player::rot_180() {return B.rotate_piece(2);}
bool Player::rot_cw() {return B.rotate_piece(1);}

void Player::hard_drop(){
    B.hard_drop();
}

void Player::swap(){
    B.swap_piece();
}

void Player::undo(){
    B.undo();
}

void Player::reset(){
    B.reset();
}