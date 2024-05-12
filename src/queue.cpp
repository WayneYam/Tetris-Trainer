
#include "queue.hpp"
#include "debug.hpp"
#include <algorithm>
#include <deque>
#include <random>

std::random_device rd;
std::mt19937 rng(rd());


std::vector<int> gen_bag(){
    std::vector<int> bag(7);
    for(int i  = 0; i < 7; i++) bag[i] = i;
    std::shuffle(bag.begin(), bag.end(), rng);
    return bag;
}

int Queue::get_piece(){
    
    while(queue.size() < 7){
        auto bag = gen_bag();
        for(auto i : bag) queue.push_back(i);
    } 

    int ret = queue.front();
    queue.pop_front();
    return ret;
}

std::deque<int> Queue::get_queue(){
    while(queue.size() < 7){
        auto bag = gen_bag();
        for(auto i : bag) queue.push_back(i);
    } 
    return queue;
}

void Queue::put_queue(std::deque<int> v){
    while(v.size()){
        queue.push_front(v.back());
        v.pop_back();
    }
}

void Queue::reset_queue(){
    queue.clear();
}
