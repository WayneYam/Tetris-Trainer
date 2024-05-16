
#include "ai.hpp"
#include <SFML/System/Sleep.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <algorithm>
#include <array>
#include <numeric>
#include <set>
#include <queue>
#include "debug.hpp"

int autoplay = 0;
PlayerAI::PlayerAI(int n, int m) : Player(n, m) {reset();}
// Disable movement
void PlayerAI::event_handler(sf::Event event){
    if (event.type == sf::Event::KeyPressed){
        if(event.key.code == get_keybind(Keybind::reset))      reset();
        if(event.key.code == get_keybind(Keybind::hard_drop))  hard_drop();
        if(event.key.code == get_keybind(Keybind::undo))       undo();
        if(event.key.code == sf::Keyboard::P) autoplay ^= 1;
    }
}

void PlayerAI::reset(){
    // debug("called reset");
    std::cout << "put " << piece_count << " pieces\n";
    Player::reset();
    Player::swap();
}

bool is_solid(int x){
    return 0 <= x && x < 7;
}

int eval(std::vector<int> v){
    int evaluation = 0;
    { // clear only tetris.
        int mn = *std::min_element(v.begin(), v.end());
        if(mn != 0){
            if(mn == 4){ // tetris
                evaluation += 50;
            }else{
                evaluation -= 100;
            }
        }
        if(v[0] != mn) { // 0-9 stacking
            evaluation -= 2000;
        }
    }


    v[0] = 100;
    v.push_back(100);

    std::vector<int> dependency(7); // Z L O S I J T
    {
        for(int i = 1; i < (int)v.size() - 1; i++){
            if(v[i] == std::min({v[i-1], v[i], v[i+1]}) ) {
                int dl = v[i-1] - v[i], dr = v[i+1] - v[i];

                if(dl >= 3 && dr >= 3){
                    dependency[4]++; // I piece
                }else if(dl == 2 && dr >= 3){
                    dependency[1]++; // L piece
                }else if(dr == 2 && dl >= 3){
                    dependency[5]++; // J piece
                }else if(dr == 2 && dl == 2){
                    // L J dependency
                }
            }
        }

        for(int i = 0; i + 4 < (int)v.size(); i++){
            for(int t : {-1, 1})
                if( (v[i] - v[i+1] == t || v[i] - v[i+1] <= -t) && v[i+1] - v[i+2] == t && v[i+2] - v[i+3] == t) dependency[6]++; // T piece
        }
    }

    int dsum = std::accumulate(dependency.begin(), dependency.end(), 0);

    if(dsum > 1) evaluation -= 10;
    if(dsum > 2) evaluation -= 20;
    if(dsum > 3) evaluation -= 30;

    for(int i : dependency){
        if(i > 1) evaluation -= 50;
        if(i > 2) evaluation -= 200;
    }

    for(int i = 1; i + 2 < (int)v.size(); i++){
        int diff = std::abs(v[i] - v[i+1]);
        evaluation -= diff * diff * 3;
        // evaluation -= diff * 5;
    }

    return evaluation;
}


void PlayerAI::place_a_piece(){
    struct comp {
        bool operator() (const Board::Piece &a, const Board::Piece &b) const {
            return std::array<int, 4>({a.px, a.py, a.t, a.rot}) < std::array<int, 4>({b.px, b.py, b.t, b.rot});
        }
    };
    
    // generate all possible board state
    std::set<Board::Piece, comp> visited;

    Board::Piece best_placement;
    int best_eval = -1;

    std::queue<Board::Piece> bfs_queue;
    Board B2 = B;
    for(int i = 0; i < 2; i++){
        B2.take(B2.current_piece);
        bfs_queue.push(B2.current_piece);
        visited.insert(B2.current_piece);
        B2.put(B2.current_piece);
        B2.swap_piece();
    }
    while(!bfs_queue.empty()){
        Board::Piece test = bfs_queue.front();
        bfs_queue.pop();
        // debug("walk", test.px, test.py, test.rot, test.t);

        for(int dir : {0, 2, 3}){
            B2.current_piece = test;
            
            B2.put(B2.current_piece);
            if(dir != 3) B2.move_piece(dir);
            else while(B2.move_piece(dir)); //infinite softdrop
            B2.take(B2.current_piece);

            if(!visited.count(B2.current_piece)){
                visited.insert(B2.current_piece);
                bfs_queue.push(B2.current_piece);
            }
        }

        for(int dir : {1, 2, 3}){
            B2.current_piece = test;
            B2.put(B2.current_piece);
            B2.rotate_piece(dir);
            B2.take(B2.current_piece);
            if(!visited.count(B2.current_piece)){
                visited.insert(B2.current_piece);
                bfs_queue.push(B2.current_piece);
            }
        }

        B2.current_piece = test;
        B2.put(B2.current_piece);

        Tiles cur_tiles =  B2.current_piece.get_tiles();
        bool touch_ground = 1;
        for(auto [x, y] : cur_tiles){
            if(B2.valid_position(x - 1, y)){
                touch_ground = 0;
            }
        }
        if(touch_ground) { // can hard_drop
            
            // debug("test", test.px, test.py, test.rot, test.t);
            std::vector<int> height(B2.M);
            for(int i = 0; i < B2.M; i++){
                for(int j = 0; j < 2 * B2.N; j++){
                    if(!is_solid(B2[j][i])){
                        height[i] = j;
                        break;
                    }
                }
            }
            int current_eval = eval(height);

            if(best_placement.t == -1 || current_eval > best_eval){
                best_placement = B2.current_piece;
                best_eval = current_eval;
            }
        }
        B2.take(B2.current_piece);
    }

    if(best_placement.t == -1) return reset();

    if(B.current_piece.t != best_placement.t) swap();

    B.take(B.current_piece);
    B.current_piece = best_placement;
    B.put(B.current_piece);

    // debug(best_eval);

    Player::hard_drop();
}


void PlayerAI::hard_drop(){
    place_a_piece();
}

void PlayerAI::do_motion(){
    if(autoplay) {
        place_a_piece();
    }
}


