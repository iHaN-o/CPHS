#include <bits/stdc++.h>
#include "internal.hpp"

using namespace std;

int main(int argc, char **argv) {
    printf("start\n");
    Internal s;
    s.opts.initial_argv(argc, argv);
    s.opts.specialknum(argv[1]);
    s.opts.show_parse();
    s.readfile(argv[1]);
    //s.simpsolve();
    s.solve();
    return 0;
}
