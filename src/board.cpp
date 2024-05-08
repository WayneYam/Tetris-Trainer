#include "board.hpp"
#include "types.hpp"
#include "const.hpp"
#include "queue.hpp"
#include "config.hpp"
#include <utility>
const int N = row_number;
const int M = column_number;
Board B(N + 3, std::vector<int>(M, -1));
int swap_state = 0;

//for garbage line generate
int total_spike=0;
#include <random>
std::default_random_engine generator( time(NULL) );
std::uniform_real_distribution<float> real_distribution(0.0, 1.0);
std::uniform_int_distribution<int> int_distribution(0,M-1);
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
struct Player{
	int piece;
	int spike;
	float time;
	float app;
	float apm;
	int spike_pos;//garbage hole's position
	int line;//how many line you clear;
	int combo;
	int btb_count;
	int garbage_line[100];
	float garbage_chaos=0.4;
	int index_start;
	int index_end;
	bool is_tspin;
	void init(){
		total_spike=0;
		piece=0;
		spike=0;
		time=0;
		app=0;
		apm=0;
		index_start=0;
		index_end=0;
		combo=-1;
		btb_count=-1;
		spike_pos=int_distribution(generator);
	}
	int spike_count(){
			int l=line;
			line=0;
			if(l==0){
					combo=-1;
					return 0;
			}
			combo++;
			int basic,btb;
			//atk=(basic+b2b)(1+combo*0.25)
			if(is_tspin){
				basic=2*l;
				btb_count++;
			}
			else if(l==4){
				basic=4;
				btb_count++;
			}
			else{
				basic=l-1;
				btb_count=-1;
			}
			if(btb_count<1) btb=0;
			else if(btb_count<3) btb=1;
			else if(btb_count<8) btb=2;
			else if(btb_count<24) btb=3;
			else if(btb_count<67) btb=4;
			else btb=5;
			if(basic+btb==0){
					if(combo<2) return 0;
					if(combo<6) return 1;
					if(combo<16) return 2;
					return 3;
			}
			return (basic+btb)*(4+combo)/4;
	}
	void garbage_gen(){
			while(index_start!=index_end){
					float x=real_distribution(generator);
					int n=garbage_line[index_start];
					if(x>garbage_chaos){
							spike_pos=spike_pos;
					}
					else{
							spike_pos=int_distribution(generator);
					}
					for(int k=0+3;k<N-n+3;k++){
							B[k]=B[k+n];
					}
					for(int k=0;k<M;k++){
						for(int j=0;j<n;j++){
							if(k!=spike_pos) B[N+2-j][k]=7;
							else B[N+2-j][k]=-1;
						}
					}
					index_start=(index_start+1)%100;
			}
	}
	void update(){
		piece++;
		garbage_line[index_end]=spike_count();
		total_spike+=garbage_line[index_end];
		index_end=(index_end+1)%100;
		garbage_gen();
	}
}player;
void init_piece(int c){
    current_piece = {0, 3, c, 0};
    if(!current_piece.valid()) reset();
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
            //for garbage sent
            player.line++;
            //
            for(int i = cur - 1; i >= 0; i--){
                B[i + 1] = B[i];
            }
            for(int &i : B[0]) i = -1;
        }else{
            cur--;
        }
    }
}

struct State{
    int cp, hp;
    Board B;
};

std::deque<State> backups;
void hard_drop(){
    current_piece.take();
    backups.push_front({current_piece.t, hold_piece.t, B});
    current_piece.put();
    while((int)backups.size() > 500) backups.pop_back();

    while(move_piece(3));
    player.is_tspin=tspin_check();
    clear_lines();
	player.update();
    init_piece(get_piece());

    swap_state = 0;
}

void undo(){
    if(backups.empty()) return;
    if(swap_state){
        std::swap(current_piece, hold_piece);
        swap_state = 0;
    }
    current_piece.take();
    put_queue({current_piece.t});
    State last = backups.front();
    backups.pop_front();

    current_piece.t = last.cp;
    hold_piece.t = last.hp;
    B = last.B;
    init_piece(current_piece.t);
}

void swap_piece(){
    current_piece.take();
    
    std::swap(current_piece, hold_piece);
    if(current_piece.t == -1) current_piece.t = get_piece();
    init_piece(current_piece.t);

    swap_state ^= 1;
}

int get_hold_piece(){
    return hold_piece.t;
}

void reset_board(){
    B.assign(N + 3, std::vector<int>(M, -1));
    current_piece = hold_piece = Piece();
}

void reset(){
    reset_board();
    reset_queue(); 
    init_board();
}


//

bool tspin_check(){//there's not tspin mini yet
	if(current_piece.t!=6) return 0;
	current_piece.take();
	current_piece.px += -1, current_piece.py += 0;
	if(current_piece.valid()){
		current_piece.px -= -1, current_piece.py -= 0;
		current_piece.put();
		return 0;
	}
	current_piece.px -= -1, current_piece.py -= 0;
	current_piece.put();
	int x=current_piece.px,y=current_piece.py,temp=0;
	if(valid_position(x,y)) temp++;
	if(valid_position(x+2,y)) temp++;
	if(valid_position(x,y+2)) temp++;
	if(valid_position(x+2,y+2)) temp++;
	if(temp<2) return 1;
	return 0;
}

//
Board getBoard(){
    return B;
}
