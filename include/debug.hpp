#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>
#ifdef DEBUG
inline void __print(int x) {std::cerr << x;}
inline void __print(long x) {std::cerr << x;}
inline void __print(long long x) {std::cerr << x;}
inline void __print(unsigned x) {std::cerr << x;}
inline void __print(unsigned long x) {std::cerr << x;}
inline void __print(unsigned long long x) {std::cerr << x;}
inline void __print(float x) {std::cerr << x;}
inline void __print(double x) {std::cerr << x;}
inline void __print(long double x) {std::cerr << x;}
inline void __print(char x) {std::cerr << '\'' << x << '\'';}
inline void __print(const char *x) {std::cerr << '\"' << x << '\"';}
inline void __print(const std::string &x) {std::cerr << '\"' << x << '\"';}
inline void __print(bool x) {std::cerr << (x ? "true" : "false");}

template<typename T, typename V>
inline void __print(const std::pair<T, V> &x) {std::cerr << '{'; __print(x.first); std::cerr << ','; __print(x.second); std::cerr << '}';}
template<typename T>
inline void __print(const T &x) {int f = 0; std::cerr << '{'; for (auto &i: x) std::cerr << (f++ ? "," : ""), __print(i); std::cerr << "}";}
inline void _print() {std::cerr << "]\n";}
template <typename T, typename... V>
inline void _print(T t, V... v) {__print(t); if (sizeof...(v)) std::cerr << ", "; _print(v...);}
#define debug(x...) std::cerr << "[" << #x << "] = [", _print(x)
template<class T> inline void pary(T l, T r) {
	while (l != r) std::cerr << *l << " ", l++;
	std::cerr << std::endl;
}
#else
#define debug(x...)
template<class T> inline void pary(T l, T r) {}
#endif

#endif
