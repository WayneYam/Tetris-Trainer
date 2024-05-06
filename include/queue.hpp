#ifndef QUEUE_H
#define QUEUE_H

#include <deque>
int get_piece();
std::deque<int> get_queue();
void reset_queue();
void put_queue(std::deque<int> q);

#endif
