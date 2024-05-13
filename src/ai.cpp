
#include "ai.hpp"
#include <SFML/System/Sleep.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <array>
#include <set>
#include <queue>
#include "debug.hpp"

PlayerAI::PlayerAI(int n, int m) : Player(n, m) {reset();}
// Disable movement
void PlayerAI::event_handler(sf::Event event){
    if (event.type == sf::Event::KeyPressed){
        if(event.key.code == get_keybind(Keybind::reset))      reset();
        if(event.key.code == get_keybind(Keybind::hard_drop))  hard_drop();
    }
}

void PlayerAI::reset(){
    // debug("called reset");
    Player::reset();
    Player::swap();
}

bool is_solid(int x){
    return 0 <= x && x < 7;
}
int eval(const Board &B){
    return 0;
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
        if(!B2.move_piece(3)) { // can hard_drop
            
            // debug("test", test.px, test.py, test.rot, test.t);
            int current_eval = eval(B2);
            if(best_placement.t == -1 || current_eval > best_eval){
                best_placement = B2.current_piece;
                best_eval = current_eval;
            }
        }
        B2.take(B2.current_piece);
    }

    if(B.current_piece.t != best_placement.t) swap();

    B.take(B.current_piece);
    B.current_piece = best_placement;
    B.put(B.current_piece);

    debug(best_eval);

    Player::hard_drop();
}


void PlayerAI::hard_drop(){
    place_a_piece();
}

void PlayerAI::do_motion(){
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::P)){
        place_a_piece();
        sf::sleep(sf::seconds(0.1));
    }
}


