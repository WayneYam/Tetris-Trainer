
#include "queue.hpp"
#include <algorithm>
#include <deque>
#include <random>

std::random_device rd;
std::mt19937 rng(rd());

std::deque<int> queue;

void gen_bag(){
    std::vector<int> bag(7);
    for(int i  = 0; i < 7; i++) bag[i] = i;
    std::shuffle(bag.begin(), bag.end(), rng);
    for(int i : bag) queue.push_back(i);
}

int get_piece(){
    while(queue.size() < 7){
        gen_bag();
    } 

    int ret = queue.front();
    queue.pop_front();
    return ret;
}

std::deque<int> get_queue(){
    while(queue.size() < 7){
        gen_bag();
    } 
    return queue;
}

void put_queue(std::deque<int> v){
    while(v.size()){
        queue.push_front(v.back());
        v.pop_back();
    }
}

void reset_queue(){
    queue.clear();
}
