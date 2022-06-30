#ifndef _options_hpp_INCLUDED
#define _options_hpp_INCLUDED

#include <cstring>
#include <assert.h>

#define OPTIONS \
\
/*      NAME         DEFAULT, LO, HI, USAGE */ \
\
OPTION( addprob, 100, 0, 1000, "use cut point probility") \
OPTION( bestres, 0, 0, 1000000000, "use cut point probility") \
OPTION( crossprob, 90, 1, 100, "cross para") \
OPTION( cutprob, 10, 0, 100, "use cut point probility") \
OPTION( knum, 200, 0, 1000000000, "critical node num") \
OPTION( maxiter, 5000, 0, 1000000, "max iter num in a round") \
OPTION( mutationlen, 50, 0, 100, "mutation para") \
OPTION( mutationpow, 8, 0, 100, "mutation para") \
OPTION( mutationprob, 10, 0, 10000, "mutation para") \
OPTION( nonimprove, 1000, 0, 100000, "nonimprove iters") \
OPTION( poolsize, 20, 1, 100, "solution pool size") \
OPTION( rankpara, 40, 1, 100, "rank para") \
OPTION( times, 3600, 0, 1000000, "cutoff time") \

struct Option {
    const char * name;
    int def, lo, hi;
    const char * description;
//    int & val (Options *);
};

static const int number_of_options = 
#define OPTION(N, V, L, H, D) 1 +
OPTIONS
#undef OPTION
+ 0;

struct Internal;

class Options {
    friend struct Option;
private:
    int values[number_of_options];
    Option table[number_of_options];

public:
    Options();

#define OPTION(N, V, L, H, D) \
    int N = V;
    OPTIONS
#undef OPTION

    void initial_argv(int argc, char **argv);
    void set_val(char *arg);
    int  has(char *name);
    void specialknum(char *file);
    void show_parse();
    
};


#endif