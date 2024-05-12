#ifndef QUEUE_H
#define QUEUE_H

#include <deque>
struct Queue{
    std::deque<int> queue;
    int get_piece();
    std::deque<int> get_queue();
    void reset_queue();
    void put_queue(std::deque<int> q);
};

#endif
