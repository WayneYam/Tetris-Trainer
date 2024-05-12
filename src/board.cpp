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



Board::Board (int n, int m) : N(n), M(m), std::vector<std::vector<int> > (n * 2, std::vector<int>(m, -1)) {};
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

void Board::init_piece(int c){
    current_piece = {N+2, 3, c, 0};
    if(!valid(current_piece)) reset();
    put(current_piece);
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
        current_piece.px -= dy;
        current_piece.py += dx;
        if(valid(current_piece)){
            put(current_piece);
            return 1;
        }
        current_piece.px += dy;
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
	if(valid_position(x+2,y)) temp++;
	if(valid_position(x,y+2)) temp++;
	if(valid_position(x+2,y+2)) temp++;
	if(temp<2) return 1;
	return 0;
}

void Board::clear_lines(){
    for(int cur = 0; cur < N + 2;){
        bool full = 1;
        for(int i = 0; i < M; i++) if((*this)[cur][i] == -1) full = 0;
        if(full){
            //for garbage sent
            // player.line++;
            //
            for(int i = cur; i < N + 2; i++){
                (*this)[i] = (*this)[i + 1];
            }
            for(int &i : (*this).back()) i = -1;
        }else{
            cur++;
        }
    }
}
void Board::hard_drop(){
    take(current_piece);
    backups.push_front({current_piece.t, hold_piece.t, (*this)});
    put(current_piece);
    while((int)backups.size() > 500) backups.pop_back();

    while(move_piece(3));
    // player.is_tspin=tspin_check();
    clear_lines();
    // player.update();
    init_piece(queue.get_piece());

    swap_state = 0;
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
    (std::vector<std::vector<int> >)(*this) = last.B;
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



const int N = row_number;
const int M = column_number;



// struct Player{
//     Board B(N, M);
// 	int piece;
// 	int spike;
// 	float time;
// 	float app;
// 	float apm;
// 	int spike_pos;//garbage hole's position
// 	int line;//how many line you clear;
// 	int combo;
// 	int btb_count;
// 	int garbage_line[100];
// 	float garbage_chaos=0.4;
// 	int index_start;
// 	int index_end;
// 	bool is_tspin;
// 	void init(){
// 		total_spike=0;
// 		piece=0;
// 		spike=0;
// 		time=0;
// 		app=0;
// 		apm=0;
// 		index_start=0;
// 		index_end=0;
// 		combo=-1;
// 		btb_count=-1;
// 		spike_pos=int_distribution(generator);
// 	}
// 	int spike_count(){
// 			int l=line;
// 			line=0;
// 			if(l==0){
// 					combo=-1;
// 					return 0;
// 			}
// 			combo++;
// 			int basic,btb;
// 			//atk=(basic+b2b)(1+combo*0.25)
// 			if(is_tspin){
// 				basic=2*l;
// 				btb_count++;
// 			}
// 			else if(l==4){
// 				basic=4;
// 				btb_count++;
// 			}
// 			else{
// 				basic=l-1;
// 				btb_count=-1;
// 			}
// 			if(btb_count<1) btb=0;
// 			else if(btb_count<3) btb=1;
// 			else if(btb_count<8) btb=2;
// 			else if(btb_count<24) btb=3;
// 			else if(btb_count<67) btb=4;
// 			else btb=5;
// 			if(basic+btb==0){
// 					if(combo<2) return 0;
// 					if(combo<6) return 1;
// 					if(combo<16) return 2;
// 					return 3;
// 			}
// 			return (basic+btb)*(4+combo)/4;
// 	}
// 	void garbage_gen(){
// 			while(index_start!=index_end){
// 					float x=real_distribution(generator);
// 					int n=garbage_line[index_start];
// 					if(x>garbage_chaos){
// 							spike_pos=spike_pos;
// 					}
// 					else{
// 							spike_pos=int_distribution(generator);
// 					}
// 					for(int k=0+3;k<N-n+3;k++){
// 							B[k]=B[k+n];
// 					}
// 					for(int k=0;k<M;k++){
// 						for(int j=0;j<n;j++){
// 							if(k!=spike_pos) B[N+2-j][k]=7;
// 							else B[N+2-j][k]=-1;
// 						}
// 					}
// 					index_start=(index_start+1)%100;
// 			}
// 	}
// 	void update(){
// 		piece++;
// 		garbage_line[index_end]=spike_count();
// 		total_spike+=garbage_line[index_end];
// 		index_end=(index_end+1)%100;
// 		garbage_gen();
// 	}
//     Board getBoard(){
//         return B;
//     }
// }player;
//
//
//
// //for garbage line generate
// int total_spike=0;
// #include <random>
// std::default_random_engine generator( time(NULL) );
// std::uniform_real_distribution<float> real_distribution(0.0, 1.0);
// std::uniform_int_distribution<int> int_distribution(0,M-1);
//
//
