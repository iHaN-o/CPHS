#include "options.hpp"
#include <cstdio>
#include <iostream>

Options::Options() {
    int i = 0;
#define OPTION(N, V, L, H, D) \
    do { \
        table[i] = {#N, (int)(V), (int)(L), (int)(H), D }; \
        values[i] = __INT_MAX__; \
        N = V; \
        ++i; \
    } while(0);
    OPTIONS
#undef OPTION
}
 
int Options::has(char *name) {
    int l = 0, r = number_of_options-1;
    while (l <= r) {
        int m = (l + r) >> 1;
        int tmp = strcmp(name, table[m].name);
        if (!tmp) return m;
        if (tmp < 0) r = m - 1;
        else l = m + 1;
    }
    return -1;
}

void Options::set_val(char *arg) {
    if (arg[0] != '-' || arg[1] != '-') return;
    int l = strlen(arg), pe = 0;
    for (int i = 2; i < l; i++)
        if (arg[i] == '=') pe = i;
    if (!pe) return;
    char name[50];
    strncpy(name, arg + 2, pe - 2);
    name[pe - 2] = '\0';
    int index = has(name);
    assert(index >= 0);
    int v = 0;
    for (int i = pe + 1; i < l; i++)
        v = v * 10 + arg[i] - '0';
    values[index] = v;
}

void Options::initial_argv(int argc, char **argv) { 
    for (int i = 0; i < argc; i++)
        set_val(argv[i]);
    int i = 0;
#define OPTION(N, V, L, H, D) \
    do { \
        if (values[i] != __INT_MAX__) \
            N = values[i]; \
        i++; \
    } while(0);
    OPTIONS
#undef OPTION
}

void Options::specialknum(char *file) {
    char name[50], p = -1, l = strlen(file);
    for (int i = l - 1; i >= 0; i--)
        if (file[i] == '/') { p = i; break; }
    strncpy(name, file + p + 1, l - p - 1);
    name[l - p - 1] = '\0';
    printf("c Ori name: %s\n", file);
    printf("c File name: %s\n", name);
    if (strcmp(name, "BarabasiAlbert_n500m1.txt" ) == 0) knum = 50, bestres = 195;
    if (strcmp(name, "BarabasiAlbert_n1000m1.txt") == 0) knum = 75, bestres = 558;
    if (strcmp(name, "BarabasiAlbert_n2500m1.txt") == 0) knum = 100, bestres = 3704;
    if (strcmp(name, "BarabasiAlbert_n5000m1.txt") == 0) knum = 150, bestres = 10196;

    if (strcmp(name, "ErdosRenyi_n250.txt" ) == 0 ) knum = 50, bestres = 295;
    if (strcmp(name, "ErdosRenyi_n500.txt" ) == 0 ) knum = 80, bestres = 1524;
    if (strcmp(name, "ErdosRenyi_n1000.txt") == 0) knum = 140, bestres = 5012;
    if (strcmp(name, "ErdosRenyi_n2500.txt") == 0) knum = 200;

    if (strcmp(name, "ForestFire_n250.txt" ) == 0) knum = 50, bestres = 194;
    if (strcmp(name, "ForestFire_n500.txt" ) == 0) knum = 110, bestres = 257;
    if (strcmp(name, "ForestFire_n1000.txt") == 0) knum = 150, bestres = 1260;
    if (strcmp(name, "ForestFire_n2000.txt") == 0) knum = 200, bestres = 4545;

    if (strcmp(name, "WattsStrogatz_n250.txt" ) == 0) knum = 70, bestres = 3083;
    if (strcmp(name, "WattsStrogatz_n500.txt" ) == 0) knum = 125, bestres = 2072;
    if (strcmp(name, "WattsStrogatz_n1000.txt") == 0) knum = 200;
    if (strcmp(name, "WattsStrogatz_n1500.txt") == 0) knum = 265;
    
    if (strcmp(name, "astroph.txt" ) == 0) knum = 1877;
    if (strcmp(name, "Bovine.txt" ) == 0) knum = 3, bestres = 268;
    if (strcmp(name, "Circuit.txt") == 0) knum = 25, bestres = 2099;
    if (strcmp(name, "condmat.txt") == 0) knum = 2313;
    if (strcmp(name, "Ecoli.txt") == 0) knum = 15, bestres = 806;

    if (strcmp(name, "EU_flights.txt" ) == 0 ) knum = 119;
    if (strcmp(name, "facebook.txt" ) == 0 ) knum = 404;
    if (strcmp(name, "grqc.txt") == 0) knum = 524;
    if (strcmp(name, "Hamilton1000.txt") == 0) knum = 100;

    if (strcmp(name, "Hamilton2000.txt" ) == 0) knum = 200;
    if (strcmp(name, "Hamilton3000a.txt" ) == 0) knum = 300;
    if (strcmp(name, "Hamilton3000b.txt") == 0) knum = 300;
    if (strcmp(name, "Hamilton3000c.txt") == 0) knum = 300;

    if (strcmp(name, "Hamilton3000d.txt" ) == 0) knum = 300;
    if (strcmp(name, "Hamilton3000e.txt" ) == 0) knum = 400;
    if (strcmp(name, "Hamilton4000.txt") == 0) knum = 400;
    if (strcmp(name, "Hamilton5000.txt") == 0) knum = 500;

    if (strcmp(name, "hepph.txt" ) == 0) knum = 1201;
    if (strcmp(name, "hepth.txt" ) == 0) knum = 988;
    if (strcmp(name, "humanDiseasome.txt") == 0) knum = 52, bestres = 1115;
    if (strcmp(name, "OClinks.txt") == 0) knum = 190;

    if (strcmp(name, "openflights.txt" ) == 0) knum = 186;
    if (strcmp(name, "powergrid.txt" ) == 0) knum = 494;
    if (strcmp(name, "Treni_Roma.txt") == 0) knum = 26, bestres = 918;
    if (strcmp(name, "USAir97.txt") == 0) knum = 33, bestres = 4336;
    if (strcmp(name, "yeast1.txt") == 0) knum = 202, bestres = 1412;
    printf("knum: %d\n", knum);
}

void Options::show_parse() {
    printf("c ---------------------- parse list ----------------------\n");
    #define OPTION(N, V, L, H, D) \
    do { \
        std::cout << "c " << #N << " = " << N << std::endl; \
    } while(0);
    OPTIONS
#undef OPTION
    printf("c ---------------------- end ----------------------\n");
}