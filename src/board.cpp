#include "board.hpp"
#include "types.hpp"
#include "const.hpp"
#include "queue.hpp"
#include "config.hpp"
#include <utility>
#include "debug.hpp"

Tiles get_dis(int c, int rotation){ // get displacement
    static const std::vector<std::vector<Tiles>> table = 
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

Tiles get_rot_table (int t, int cr, int nr) {
    Tiles test;
    if(t == 4){ // I
        // tetrio SRS+ kick table
        if(cr == 0 && nr == 1) test = {{0, 0}, { 1, 0}, {-2,  0}, {-2, -1}, { 1,  2}};
        if(cr == 1 && nr == 0) test = {{0, 0}, {-1, 0}, { 2,  0}, {-1, -2}, { 2,  1}};
        if(cr == 1 && nr == 2) test = {{0, 0}, {-1, 0}, { 2,  0}, {-1,  2}, { 2, -1}};
        if(cr == 2 && nr == 1) test = {{0, 0}, {-2, 0}, { 1,  0}, {-2,  1}, { 1, -2}};
        if(cr == 2 && nr == 3) test = {{0, 0}, { 2, 0}, {-1,  0}, { 2,  1}, {-1, -2}};
        if(cr == 3 && nr == 2) test = {{0, 0}, { 1, 0}, {-2,  0}, { 1,  2}, {-2, -1}};
        if(cr == 3 && nr == 0) test = {{0, 0}, { 1, 0}, {-2,  0}, { 1, -2}, {-2,  1}};
        if(cr == 0 && nr == 3) test = {{0, 0}, {-1, 0}, { 2,  0}, { 2, -1}, {-1,  2}};

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
    return test;
}



Board::Board (int n, int m) : std::vector<std::vector<int> > (n * 2, std::vector<int>(m, -1)), N(n), M(m) {};
Board::Piece::Piece () {px = py = t = rot = -1;}
Board::Piece::Piece(int a, int b, int c, int d){ px = a, py = b, t = c, rot = d; }

Tiles Board::Piece::get_tiles(){
    Tiles dis = get_dis(t, rot);
    for(auto &[x, y] : dis){
        x = px - x, y = py + y;
    }
    return dis;
}


bool Board::valid_position(int x, int y){
    return 0 <= x && x < N + 3 && 0 <= y && y < M && ( (*this)[x][y] == -1 || (*this)[x][y] >= 10);
}

void Board::put(Piece p){
    if(get_option(Option::ghost)) {
        Piece ghost = p;
        if(valid(ghost)){
            while(valid(ghost)) ghost.px--;
            ghost.px++;
            for(auto [x, y] : ghost.get_tiles()){
                (*this)[x][y] = p.t + 10;
            }
        }
    }

    for(auto [x, y] : p.get_tiles()){
        (*this)[x][y] = p.t;
    }
}
void Board::take(Piece p){
    for(auto [x, y] : p.get_tiles()){
        (*this)[x][y] = -1;
    }
    if(get_option(Option::ghost)) {
        Piece ghost = p;
        if(valid(ghost)){
            while(valid(ghost)) ghost.px--;
            ghost.px++;
            for(auto [x, y] : ghost.get_tiles()){
                (*this)[x][y] = -1;
            }
        }
    }
}
bool Board::valid(Piece p){
    for(auto [x, y] : p.get_tiles()){
        if(!valid_position(x, y)) return 0;
    }
    return 1;
}

int Board::init_piece(int c){
    current_piece = {N+2, 3, c, 0};
    if(!valid(current_piece)) return 1;
    put(current_piece);
    return 0;
}

void Board::init_board(){
    init_piece(queue.get_piece());
}


bool Board::move_piece(int dir){
    const std::vector<std::pair<int, int> > vec = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}}; // R U L D
    take(current_piece);
    auto [dx, dy] = vec[dir];
    current_piece.px += dx, current_piece.py += dy;
    bool ok = valid(current_piece);
    if(!ok) current_piece.px -= dx, current_piece.py -= dy;
    put(current_piece);
    return ok;
}

bool Board::rotate_piece(int cnt){ 
    if(cnt == 0) return 0;

    Piece backup = current_piece;
    take(current_piece);

    int cr = backup.rot;
    int nr = (cr+cnt)%4;
    Tiles test = get_rot_table(current_piece.t, cr, nr);
    current_piece.rot = nr;
    for(auto [dx, dy] : test){
        current_piece.px += dy;
        current_piece.py += dx;
        if(valid(current_piece)){
            put(current_piece);
            return 1;
        }
        current_piece.px -= dy;
        current_piece.py -= dx;
    }

    current_piece = backup;
    put(current_piece);
    return 0;
}
bool Board::tspin_check(){//there's not tspin mini yet
	if(current_piece.t!=6) return 0;
	take(current_piece);
	current_piece.px += -1, current_piece.py += 0;
	if(valid(current_piece)){
		current_piece.px -= -1, current_piece.py -= 0;
		put(current_piece);
		return 0;
	}
	current_piece.px -= -1, current_piece.py -= 0;
	put(current_piece);
	int x=current_piece.px,y=current_piece.py,temp=0;
	if(valid_position(x,y)) temp++;
	if(valid_position(x-2,y)) temp++;
	if(valid_position(x,y+2)) temp++;
	if(valid_position(x-2,y+2)) temp++;
	if(temp<2) return 1;
	return 0;
}

int Board::clear_lines(){
    int res = 0;
    for(int cur = 0; cur < N + 2;){
        bool full = 1;
        for(int i = 0; i < M; i++) if((*this)[cur][i] == -1) full = 0;
        if(full){
            res++;
            for(int i = cur; i < N + 2; i++){
                (*this)[i] = (*this)[i + 1];
            }
            for(int &i : (*this).back()) i = -1;
        }else{
            cur++;
        }
    }
    return res;
}
Lineclear Board::hard_drop(){
    take(current_piece);
    backups.push_front({current_piece.t, hold_piece.t, (*this)});
    put(current_piece);
    while((int)backups.size() > 500) backups.pop_back();

    while(move_piece(3));
    Lineclear ret;
    ret.tspin= tspin_check();
    ret.lines = clear_lines();
    ret.is_pc = 1;
    for(int i = 0; i < N; i++){
        for(int j = 0; j < M; j++){
            if(!valid_position(i, j)) ret.is_pc = 0;
        }
    }
    swap_state = 0;

    return ret;
}

void Board::undo(){
    if(backups.empty()) return;
    if(swap_state){
        std::swap(current_piece, hold_piece);
        swap_state = 0;
    }
    take(current_piece);
    queue.put_queue({current_piece.t});
    State last = backups.front();
    backups.pop_front();

    current_piece.t = last.cp;
    hold_piece.t = last.hp;
    for(int i = 0; i < (int)last.B.size(); i++) (*this)[i] = last.B[i];
    init_piece(current_piece.t);
}

void Board::swap_piece(){
    take(current_piece);

    std::swap(current_piece, hold_piece);
    if(current_piece.t == -1) current_piece.t = queue.get_piece();
    init_piece(current_piece.t);

    swap_state ^= 1;
}

int Board::get_hold_piece(){
    return hold_piece.t;
}

void Board::reset_board(){
    (*this).assign(N * 2, std::vector<int>(M, -1));
    current_piece = hold_piece = Piece();
}
void Board::reset(){
    reset_board();
    queue.reset_queue(); 
    init_board();
}


