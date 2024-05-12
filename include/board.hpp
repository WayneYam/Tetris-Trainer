#ifndef BOARD_H
#define BOARD_H

#include "types.hpp"
#include "queue.hpp"
#include <deque>


struct Board : public std::vector<std::vector<int> > {
    Board(int n, int m);
    Queue queue;
    int swap_state = 0, can_undo = 1;
    int N, M;
    struct State{
        int cp, hp;
        std::vector<std::vector<int> >  B;
    };
    std::deque<State> backups;

    struct Piece{
        int px, py, t, rot;
        Tiles get_tiles();
        Piece();
        Piece(int a, int b, int c, int d);

    } current_piece, hold_piece;
    
    bool valid_position(int x, int y);
    void put(Piece p);
    void take(Piece p);
    bool valid(Piece p);
    int init_piece(int c);
    void init_board();
    bool move_piece(int dir);
    bool rotate_piece(int cnt); 
    bool tspin_check();
    int clear_lines();
    Lineclear hard_drop();
    void undo();
    void swap_piece();
    int get_hold_piece();
    void reset_board();
    void reset();

};


Tiles get_dis(int c, int rotation); 
Board getBoard();

bool tspin_check();
#endif

