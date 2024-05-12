#ifndef TYPES_H
#define TYPES_H

#include <vector>
using Tiles = std::vector<std::pair<int, int> >;
struct Lineclear{
    int tspin, lines, is_pc; // 0: none, 1: mini, 2: normal
    Lineclear(int a = 0, int b = 0, int c = 0) : tspin(a), lines(b), is_pc(c) {}
};

#endif
