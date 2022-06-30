#ifndef _internal_hpp_INCLUDED
#define _internal_hpp_INCLUDED


#include <bits/stdc++.h>
#include "options.hpp"
#include "vec.hpp"

typedef long long ll;

struct Internal {
    Options opts;
    int nnum, mnum, knum;
    int timestamp, nblock, ncut, best_val, counter, impnum, updnum, deleteFlag;
    double avgiter, avgsim;
    int choose_best_cut;
    int *sol, *insol, *seen, *best_node, *belong, *weight, *cutnode, *inque;
    int *dfn, *low, *score, *son, *cutson, *cutscore, *checkseen;
    int **pool, *samenode;
    std::chrono::high_resolution_clock::time_point best_time;
    vec<int> *edge;
    vec<int> *block;

    int count = 0;
    Internal();

    void tarjan(int u, int fa, int rt);
    void deletenode(int num);
    int findcut(int pick);
    void readfile(char *file);
    int  getint();
    void findblock();
    void seefrom(int x, int nblock);
    void simpsolve();
    void mutation(double similar);
    int improve();
    int selectblock();
    void addnode(int x);
    void deletenode();
    void dyefrom(int x, int nblock);
    void allocmemory();
    void solve();
    void cross(int x, int y);
    void updatepool();
    double calsimilar();

    
    int checkseenfrom(int x);
};

#endif