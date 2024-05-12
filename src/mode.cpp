
#include "mode.hpp"
#include "debug.hpp"

void PlayerBackfire::hard_drop(){
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
    garbage.push(dam);
    attack_count += dam;
    if(ret.lines == 0){
        garbage_gen();
    }
    if(B.init_piece(B.queue.get_piece())) reset();
}

PlayerCheese::PlayerCheese(int n, int m) : Player(n, m) {
    reset();
}

void PlayerCheese::hard_drop(){
    auto ret = B.hard_drop();
    piece_count++;
    if(ret.lines == 0){
        garbage_gen();
    }
    gen_cheese_to(3);
    if(B.init_piece(B.queue.get_piece())){
        for(auto i : B) debug(i);
        reset();
    }
}

int PlayerCheese::current_cheese_layer(){
    int tmp = 0;
    for(int i = 0; i < B.N; i++){
        int has_cheese = 0;
        for(int j = 0; j < B.M; j++){
            if(B[i][j] == 7) // cheese
                has_cheese = 1;
        }
        tmp += has_cheese;
    }
    return tmp;
}

void PlayerCheese::gen_cheese_to(int n){
    int k = std::max(0, n - current_cheese_layer());
    while(k--){
        int nsp;
        do{
            nsp = int_distribution(generator);
        }while(nsp == spike_pos);
        spike_pos = nsp;
        for (int k = 2 * B.N - 2; k >= 0; k--) {
            B[k + 1] = B[k];
        }
        for (int k = 0; k < B.M; k++) {
            if (k != spike_pos)
                B[0][k] = 7;
            else
                B[0][k] = -1;
        }
    }
}

void PlayerCheese::garbage_gen() {gen_cheese_to(9);}

void PlayerCheese::reset(){
    Player::reset();
    garbage_gen();
    B.current_piece.px += max_layer;
    B.take(B.current_piece);
    B.current_piece.px -= max_layer;
    B.init_piece(B.current_piece.t);
}
