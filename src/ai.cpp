
#include "ai.hpp"
#include <SFML/System/Sleep.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <array>
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
        if(event.key.code == sf::Keyboard::P)  autoplay ^= 1;
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
    int evaluation = 100;

    // for(auto i : B) debug(i);
    // tuck test
    for(int i = 0; i < 2 * B.N - 1; i++){
        for(int j = 0; j < B.M; j++) {
            if(is_solid(B[i+1][j]) && !is_solid(B[i][j])) { // has to tuck
                evaluation -= 1000;
            }
        }
    }

    // hole test
    for(int i = 1; i < 2 * B.N - 1; i++){
        for(int j = 0; j < B.M; j++) {
            if(is_solid(B[i+1][j]) && !is_solid(B[i][j]) && (i == 1 || is_solid(B[i-1][j])) && (i == B.M - 1 || is_solid(B[i+1][j])) ) { // has hole
                evaluation -= 3000;
            }
        }
    }

    // find the stack height
    std::vector<int> height(B.M);
    for(int i = 0; i < B.M; i++){
        for(int j = 0;;j++){
            if(!is_solid(B[j][i])){
                height[i] = j;
                break;
            }
        }
    }


    int stack_top = *std::max_element(height.begin() + 1, height.end());
    int stack_bottom = *std::min_element(height.begin() + 1, height.end());
    // don't cover the well
    {
        int tmp = 0;
        for(int i = 0; i < 2 * B.N; i++){
            tmp += is_solid(B[i][0]);
        }
        // debug(tmp, height);
        if(tmp == 0) {}
        else if(tmp < 4) evaluation -= 10000;
        else if(tmp == 4 && (stack_bottom < 4)) evaluation -= 10000;
        else evaluation += 50; // clear the tetris
    }

    // prefer flat placement

    evaluation -= 3 * (stack_top - stack_bottom);
    for(int i = 1; i < B.M - 1; i++){
        int base_penalty = std::abs(height[i] - height[i+1]) * std::abs(height[i] - height[i+1]);
        int edge_penalty = (B.M * B.M / 2) - (i - 1) * (B.M - 1 - i);
        evaluation -=  base_penalty * base_penalty * edge_penalty / 5;
    }
    // debug(evaluation);

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
    if(autoplay){
        place_a_piece();
        // sf::sleep(sf::seconds(0.1));
    }
}


