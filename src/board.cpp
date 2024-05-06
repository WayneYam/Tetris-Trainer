#include "types.hpp"
#include "const.hpp"
#include "queue.hpp"
#include "config.hpp"
#include <utility>

const int N = row_number;
const int M = column_number;
Board B(N + 3, std::vector<int>(M, -1));

Tiles get_dis(int c, int rotation){ // get displacement

    std::vector<std::vector<Tiles>> table = 
    {
        // 'Z'
        {
            {{0, 0}, {0, 1}, {1, 1}, {1, 2}},
            {{0, 2}, {1, 1}, {1, 2}, {2, 1}},
            {{1, 0}, {1, 1}, {2, 1}, {2, 2}},
            {{0, 1}, {1, 0}, {1, 1}, {2, 0}},
        },
        // 'L'
        {
            {{1, 0}, {1, 1}, {1, 2}, {0, 2}},
            {{0, 1}, {1, 1}, {2, 1}, {2, 2}},
            {{1, 0}, {1, 1}, {1, 2}, {2, 0}},
            {{0, 1}, {1, 1}, {2, 1}, {0, 0}},
        },
        // 'O'
        {
            {{0, 1}, {0, 2}, {1, 1}, {1, 2}},
            {{0, 1}, {0, 2}, {1, 1}, {1, 2}},
            {{0, 1}, {0, 2}, {1, 1}, {1, 2}},
            {{0, 1}, {0, 2}, {1, 1}, {1, 2}},
        },
        // 'S'
        {
            {{1, 0}, {1, 1}, {0, 1}, {0, 2}},
            {{0, 1}, {1, 1}, {1, 2}, {2, 2}},
            {{2, 0}, {1, 1}, {2, 1}, {1, 2}},
            {{0, 0}, {1, 0}, {1, 1}, {2, 1}},
        },
        // 'I'
        {
            {{1, 0}, {1, 1}, {1, 2}, {1, 3}},
            {{0, 2}, {1, 2}, {2, 2}, {3, 2}},
            {{2, 0}, {2, 1}, {2, 2}, {2, 3}},
            {{0, 1}, {1, 1}, {2, 1}, {3, 1}},
        },
        // 'J'
        {
            {{1, 0}, {1, 1}, {1, 2}, {0, 0}},
            {{0, 1}, {1, 1}, {2, 1}, {0, 2}},
            {{1, 0}, {1, 1}, {1, 2}, {2, 2}},
            {{0, 1}, {1, 1}, {2, 1}, {2, 0}},
        },
        // 'T'
        {
            {{1, 1}, {1, 0}, {0, 1}, {1, 2}},
            {{1, 1}, {2, 1}, {0, 1}, {1, 2}},
            {{1, 1}, {2, 1}, {1, 0}, {1, 2}},
            {{1, 1}, {2, 1}, {1, 0}, {0, 1}},
        },
    };

    return table[c][rotation];
}

bool valid_position(int x, int y){
    return 0 <= x && x < N + 3 && 0 <= y && y < M && (B[x][y] == -1 || B[x][y] >= 10);
}

struct Piece{
    int px, py, t, rot;
    Tiles get_tiles(){
        Tiles dis = get_dis(t, rot);
        for(auto &[x, y] : dis){
            x += px, y += py;
        }
        return dis;
    }

    void put(){
        if(get_option(Option::ghost)) {
            Piece ghost = *this;
            if(ghost.valid()){
                while(ghost.valid()) ghost.px++;
                ghost.px--;
                for(auto [x, y] : ghost.get_tiles()){
                    B[x][y] = t + 10;
                }
            }
        }

        for(auto [x, y] : get_tiles()){
            B[x][y] = t;
        }
    }
    void take(){
        for(auto [x, y] : get_tiles()){
            B[x][y] = -1;
        }
        if(get_option(Option::ghost)) {
            Piece ghost = *this;
            if(ghost.valid()){
                while(ghost.valid()) ghost.px++;
                ghost.px--;
                for(auto [x, y] : ghost.get_tiles()){
                    B[x][y] = -1;
                }
            }
        }
    }
    bool valid(){
        for(auto [x, y] : get_tiles()){
            if(!valid_position(x, y)) return 0;
        }
        return 1;
    }
    Piece(){
        px = py = t = rot = -1;
    }
    Piece(int a, int b, int c, int d){
        px = a, py = b, t = c, rot = d;
    }

} current_piece, hold_piece;

void init_piece(int c){
    current_piece = {0, 3, c, 0};
    current_piece.put();
}

void init_board(){
    init_piece(get_piece());
}


bool move_piece(int dir){
    const std::vector<std::pair<int, int> > vec = {{0, 1}, {-1, 0}, {0, -1}, {1, 0}}; // R U L D
    current_piece.take();
    auto [dx, dy] = vec[dir];
    current_piece.px += dx, current_piece.py += dy;
    bool ok = current_piece.valid();
    if(!ok) current_piece.px -= dx, current_piece.py -= dy;
    current_piece.put();
    return ok;
}

bool rotate_piece(int cnt){ 
    if(cnt == 0) return 0;

    Piece backup = current_piece;
    current_piece.take();

    int cr = backup.rot;
    int nr = (cr+cnt)%4;
    
    Tiles test;
    if(current_piece.t == 4){ // I
        // tetrio SRS+ kick table
        if(cr == 0 && nr == 1) test = {{0, 0}, { 1, 0}, {-2,  0}, { 1,  2}, {-2, -1}};
        if(cr == 1 && nr == 0) test = {{0, 0}, {-1, 0}, { 2,  0}, {-1, -2}, { 2,  1}};
        if(cr == 1 && nr == 2) test = {{0, 0}, {-1, 0}, { 2,  0}, {-1,  2}, { 2, -1}};
        if(cr == 2 && nr == 1) test = {{0, 0}, {-2, 0}, { 1,  0}, {-2,  1}, { 1, -2}};
        if(cr == 2 && nr == 3) test = {{0, 0}, { 2, 0}, {-1,  0}, { 2,  1}, {-1, -2}};
        if(cr == 3 && nr == 2) test = {{0, 0}, { 1, 0}, {-2,  0}, { 1,  2}, {-2, -1}};
        if(cr == 3 && nr == 0) test = {{0, 0}, { 1, 0}, {-2,  0}, { 1, -2}, {-2,  1}};
        if(cr == 0 && nr == 3) test = {{0, 0}, {-1, 0}, { 2,  0}, {-1,  2}, { 2, -1}};

        // 180 spin on tetrio
        if(cr == 0 && nr == 2) test = {{0, 0}, { 0,-1}};
        if(cr == 2 && nr == 0) test = {{0, 0}, { 0, 1}};
        if(cr == 1 && nr == 3) test = {{0, 0}, { 1, 0}};
        if(cr == 3 && nr == 1) test = {{0, 0}, {-1, 0}};
        
    }else{
        if(cr == 0 && nr == 1) test = {{0, 0}, {-1, 0}, {-1,  1}, { 0, -2}, {-1, -2}};
        if(cr == 1 && nr == 0) test = {{0, 0}, { 1, 0}, { 1, -1}, { 0,  2}, { 1,  2}};
        if(cr == 1 && nr == 2) test = {{0, 0}, { 1, 0}, { 1, -1}, { 0,  2}, { 1,  2}};
        if(cr == 2 && nr == 1) test = {{0, 0}, {-1, 0}, {-1,  1}, { 0, -2}, {-1, -2}};
        if(cr == 2 && nr == 3) test = {{0, 0}, { 1, 0}, { 1,  1}, { 0, -2}, { 1, -2}};
        if(cr == 3 && nr == 2) test = {{0, 0}, {-1, 0}, {-1, -1}, { 0,  2}, {-1,  2}};
        if(cr == 3 && nr == 0) test = {{0, 0}, {-1, 0}, {-1, -1}, { 0,  2}, {-1,  2}};
        if(cr == 0 && nr == 3) test = {{0, 0}, { 1, 0}, { 1,  1}, { 0, -2}, { 1, -2}};

        // 180 spin on tetrio
        if(cr == 0 && nr == 2) test = {{0, 0}, { 0, 1}, { 1,  1}, { -1, 1}, { 1,  0}, {-1, 0}};
        if(cr == 2 && nr == 0) test = {{0, 0}, { 0,-1}, {-1, -1}, {  1,-1}, {-1,  0}, { 1, 0}};
        if(cr == 1 && nr == 3) test = {{0, 0}, { 1, 0}, { 1,  2}, {  1, 1}, { 0,  2}, { 0, 1}};
        if(cr == 3 && nr == 1) test = {{0, 0}, {-1, 0}, {-1,  2}, { -1, 1}, { 0,  2}, { 0, 1}};
    }

    current_piece.rot = nr;
    for(auto [dx, dy] : test){
        current_piece.px -= dy;
        current_piece.py += dx;
        if(current_piece.valid()){
            current_piece.put();
            return 1;
        }
        current_piece.px += dy;
        current_piece.py -= dx;
    }

    current_piece = backup;
    current_piece.put();
    return 0;
}

void clear_lines(){
    for(int cur = N + 2; cur > 2;){
        bool full = 1;
        for(int i = 0; i < M; i++) if(B[cur][i] == -1) full = 0;
        if(full){
            for(int i = cur - 1; i >= 0; i--){
                B[i + 1] = B[i];
            }
            for(int &i : B[0]) i = -1;
        }else{
            cur--;
        }
    }
}

void hard_drop(){
    while(move_piece(3));
    clear_lines();
    init_piece(get_piece());
}

void swap_piece(){
    current_piece.take();
    std::swap(current_piece, hold_piece);
    if(current_piece.t == -1) current_piece.t = get_piece();
    init_piece(current_piece.t);
}

int get_hold_piece(){
    return hold_piece.t;
}

void reset_board(){
    B.assign(N + 3, std::vector<int>(M, -1));
    current_piece = hold_piece = Piece();
}

Board getBoard(){
    return B;
}
