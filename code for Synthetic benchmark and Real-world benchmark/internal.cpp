#include "internal.hpp"
#include <bits/stdc++.h>
#include <chrono>
#include <sys/timeb.h>

using namespace std;

Internal::Internal() {
    nnum = 0;
    mnum = 0;
    impnum = 0;
    avgiter = avgsim = deleteFlag = 0;
    best_val = __INT_MAX__;
}

int Internal::getint() {
    int i = 0;
    char c = getchar();
    while (c < '0' || c > '9') c = getchar();
    while (c >= '0' && c <= '9')
        i = i * 10 + c - '0', c = getchar();
    return i;
}


void Internal::allocmemory() {
    sol = new int[nnum + 1];
    insol = new int[nnum + 1];
    best_node = new int[knum + 1];
    seen = new int[nnum + 1];
    belong = new int[nnum + 1];
    weight = new int[nnum + 1];
    cutnode = new int[nnum + 1];
    dfn = new int[nnum + 1];
    low = new int[nnum + 1];
    inque = new int[nnum + 1];
    samenode = new int[opts.poolsize + 1];
    score = new int[opts.poolsize + 1];
    block = new vec<int>[nnum + 1];
    son = new int[nnum + 1];
    cutson = new int[nnum + 1];
    cutscore = new int[nnum + 1];
    checkseen = new int[nnum + 1];

    pool = new int *[opts.poolsize + 1];
    for (int i = 0; i <= opts.poolsize; i++)
        pool[i] = new int[knum + 1];

}

void Internal::readfile(char *file) {
    freopen(file, "r", stdin);
    nnum = getint();
    knum = opts.knum;
    edge = new vec<int>[nnum + 1];
    for (int i = 1; i <= nnum; i++) {
        int v = getint(), h = 0;
        v = 0;
        char c = getchar();
        while (c != '\n') {
            if (c >= '0' && c <= '9') v = v * 10 + c - '0', h = 1;
            if (c == ' ' && h)
                edge[i].push(v + 1), ++mnum, v = h = 0;
            c = getchar();
        }
        if (h) edge[i].push(v + 1), ++mnum;
    }
    printf("c Graph size: %d nodes, %d edges, %d critial nodes\n", nnum, mnum, knum);
}

void Internal::seefrom(int x, int nblock) {
    block[nblock].push(x);
    seen[x] = timestamp;
    belong[x] = nblock;
    for (int i = 0; i < edge[x].size(); i++)
        if (seen[edge[x][i]] != timestamp && !insol[edge[x][i]]) seefrom(edge[x][i], nblock);
}

void Internal::dyefrom(int x, int nblock) {
    belong[x] = nblock;
    seen[x] = timestamp;
    for (int i = 0; i < edge[x].size(); i++)
        if (seen[edge[x][i]] != timestamp && !insol[edge[x][i]]) dyefrom(edge[x][i], nblock);
}

void Internal::findblock() {
    nblock = 0;
    for (int i = 1; i <= nnum; i++) {
        if (seen[i] == timestamp || insol[i]) continue;
        block[++nblock].clear();
        seefrom(i, nblock);
    }
}

int Internal::selectblock() {
    int mn = block[1].size(), mx = block[1].size();
    for (int i = 1; i <= nblock; i++) {
        if (block[i].size() > mx) mx = block[i].size();
        if (block[i].size() < mx) mn = block[i].size();
    }
    int avg = (mx + mn) >> 1, nlar = 0;
    for (int i = 1; i <= nblock; i++)
        if (block[i].size() >= avg) nlar++;
    int select = rand() % nlar + 1;
    for (int i = 1; i <= nblock; i++)
        if (block[i].size() >= avg) {
            --select;
            if (!select) return i;
        }
    assert(true);
}

void Internal::tarjan(int u, int fa, int rt) {
    inque[u] = son[u] = cutson[u] = 1;
    dfn[u] = low[u] = ++counter;
    int nson = 0, is = 0;
    for (int i = 0; i < edge[u].size(); i++) {
        int v = edge[u][i];
        if (insol[v]) continue;
        if (inque[v]) {
            if (dfn[v] < dfn[u] && v != fa) low[u] = min(low[u], dfn[v]);
            continue;
        }
        tarjan(v, u, rt);
        son[u] += son[v];
        low[u] = min(low[u], low[v]);
        nson++;
        if (u != rt && low[v] >= dfn[u]) is = 1, cutscore[u] += son[v] * (son[v] - 1) / 2, cutson[u] += son[v]; // add
        if (u == rt) cutscore[u] += son[v] * (son[v] - 1) / 2, cutson[u] += son[v];
    }
    if (u == rt && nson > 1) cutnode[++ncut] = u;
    if (is) cutnode[++ncut] = u;
}

int Internal::checkseenfrom(int x) {
    checkseen[x] = 1;
    int st = 1;
    for (int i = 0; i < edge[x].size(); i++)
        if (!checkseen[edge[x][i]] && !insol[edge[x][i]]) st += checkseenfrom(edge[x][i]);
    return st;
}

int Internal::findcut(int pick) {
    choose_best_cut = rand() % 2;
    for (int i = 0; i < block[pick].size(); i++)
        inque[block[pick][i]] = cutscore[block[pick][i]] = 0;
    ncut = 0, counter = 0;
    tarjan(block[pick][0], -1, block[pick][0]);
    
    if (ncut == 0) {
        int mx = -1;
        for (int i = 0; i < block[pick].size(); i++) {
            if (weight[block[pick][i]] > mx) ncut = 0, mx = weight[block[pick][i]];
            if (weight[block[pick][i]] == mx) cutnode[++ncut] = block[pick][i];
        }    
        return cutnode[rand() % ncut + 1];
    }
    if (!choose_best_cut)
        return cutnode[rand() % ncut + 1];
    int mns = 2e9, mn = -1;
    for (int i = 0; i < block[pick].size(); i++) {
        int u = block[pick][i];
        cutscore[u] += (block[pick].size() - cutson[u]) * (block[pick].size() - cutson[u] - 1) / 2;
        if (cutscore[u] < mns) mns = cutscore[u], mn = u;
    }
    if (mn == -1) printf("WRONG\n");
    assert(mn != -1);
    //check cutscore
    // int sc = (block[pick].size() - 1) * (block[pick].size() - 2) / 2;
    // printf("check start\n");
    // for (int i = 0; i < block[pick].size(); i++) {
    //     int u = block[pick][i];
    //     for (int j = 0; j < block[pick].size(); j++) 
    //         checkseen[block[pick][j]] = 0;
    //     checkseen[u] = 1;
    //     int s = 0;
    //     for (int j = 0; j < block[pick].size(); j++) {
    //         int v = block[pick][j];
    //         if (checkseen[v]) continue;
    //         int st = checkseenfrom(v);
    //         s += st * (st - 1) / 2;
    //     }
    //     if (s != cutscore[u]) printf("%d %d %d %d %d-%d\n", i, u, cutscore[u], s, block[pick].size(), son[u]);
    //     assert(s == cutscore[u]); 
    //     printf("\tright %d\n", s == sc ? 0 : 1);        
    // }
    // printf("check right\n");
    
    return mn;
    //return cutnode[rand() % ncut + 1];
    
}

void Internal::addnode(int x) {
    int pick = selectblock(), addp;
    int picksz = block[pick].size();
    //two strategy: random or cutpoint
    if (x == 1) {
        //printf("cut...\n");
        addp = findcut(pick);
    } 
    else {
        //can improve
        int h = 0, mx = -1;
        for (int i = 0; i < picksz; i++) {
            if (weight[block[pick][i]] > mx) h = 0, mx = weight[block[pick][i]];
            if (weight[block[pick][i]] == mx) cutnode[++h] = block[pick][i];
        }
        addp = cutnode[rand() % h + 1];
    }
    //printf("c add %d", addp);
    //updata weight
    for (int i = 0; i < picksz; i++)
        if (addp != block[pick][i]) ++weight[block[pick][i]];
    weight[addp] = 0;
    //update block
    ++timestamp;
    insol[addp] = 1, sol[knum + 1] = addp;
    for (int i = 0; i < picksz; i++) {
        int v = block[pick][i];
        if (seen[v] == timestamp || insol[v]) continue;
        block[++nblock].clear();
        seefrom(v, nblock);
    }
    block[pick].clear();
    for (int i = 0; i < block[nblock].size(); i++) {
        block[pick].push(block[nblock][i]);
        belong[block[nblock][i]] = pick;
    }
    block[nblock--].clear();
}

void Internal::deletenode(int num) {
    //random_shuffle(sol + 1, sol + num + 1);
    random_shuffle(sol + 1, sol + num + 1 - deleteFlag);
    //choose node with minimum delta
    int prescore = 0, aftscore = 0;
    for (int i = 1; i <= nblock; i++)
        prescore += block[i].size() * (block[i].size() - 1) / 2;
    int mnval = __INT_MAX__, delp = 0, pos = 0;
    for (int i = 1; i <= num - deleteFlag; i++) {
        int x = sol[i], delta = 0, comb = 1;
        ++timestamp;
        for (int j = 0; j < edge[x].size(); j++) {
            int v = belong[edge[x][j]];
            if (insol[edge[x][j]] || seen[v] == timestamp) continue;
            comb += block[v].size(), seen[v] = timestamp;
            delta -= block[v].size() * (block[v].size() - 1) / 2;
        }
        delta += comb * (comb - 1) / 2;
        if (delta < mnval) mnval = delta, delp = x, pos = i;
    }
    //printf(" del %d\n", delp);
    //update block
    sol[pos] = sol[num];
    insol[delp] = weight[delp] = 0;
    ++timestamp;
    dyefrom(delp, nblock + 1);
    for (int i = 1; i <= nblock + 1; i++)
        block[i].clear(), seen[i] = 0;
    nblock = 0;
    for (int i = 1; i <= nnum; i++) {
        if (insol[i]) continue;
        if (!seen[belong[i]]) seen[belong[i]] = ++nblock;
        int b = seen[belong[i]];
        block[b].push(i);
        belong[i] = b;
    }
    timestamp = max(timestamp, nblock + 1);
    for (int i = 1; i <= nblock; i++)
        aftscore += block[i].size() * (block[i].size() - 1) / 2;
    if (aftscore != prescore + mnval)
        printf("c %d %d %d\n", prescore, aftscore, mnval);
    assert(aftscore == prescore + mnval);
}

int Internal::improve() {
    // printf("c now sol:");
    // for (int i = 1; i <= knum; i++) printf(" %d", sol[i]);
    // puts("");
    int nowIter = 0;
    ll round_best = 0;
    int round_node[knum + 1], rounditer = 0;
    timestamp = 1;
    for (int i = 1; i <= nnum; i++) seen[i] = weight[i] = 0;
    for (int i = 1; i <= nblock; i++)
        round_best += block[i].size() * (block[i].size() - 1) / 2;
    for (int i = 1; i <= knum; i++)
        round_node[i] = sol[i];
    //printf("%d\n", round_best);
    while (++nowIter <= rounditer + opts.nonimprove) {
        // printf("%d\n", nowIter);
        //two phase: first add node to S, then delete node from S
        if (nowIter > rounditer + opts.addprob) addnode(0);
        else addnode(1);
        deleteFlag = 1;
        deletenode(knum + 1);
        deleteFlag = 0;
        //update best val in this round
        ll nowscore = 0;
        for (int i = 1; i <= nblock; i++)
            nowscore += block[i].size() * (block[i].size() - 1) / 2;
        //printf("c %d\n", nowscore);
        if (nowscore < round_best) {
            // printf("c Round %d improve: %d\n", nowIter, nowscore);
            round_best = nowscore, rounditer = nowIter;
            for (int i = 1; i <= knum; i++) round_node[i] = sol[i];
        }
    }
    ++impnum;
    avgiter = 1.0 * (avgiter * (impnum - 1) + nowIter) / impnum;
    if (impnum % 1000 == 0) {
        printf("c %d avgiter: %.2lf\n", impnum, avgiter);
        avgiter = 0;
        impnum = 0;
    }
    //update best val
    if (round_best < best_val) {
        printf("c improve: %d\n", round_best);
        best_time = std::chrono::high_resolution_clock::now();
        best_val = round_best;
        for (int i = 1; i <= knum; i++)
            best_node[i] = round_node[i];
        if (best_val <= opts.bestres) return best_val;
    }
    //need ?
    for (int i = 1; i <= nnum; i++) insol[i] = seen[i] = 0;
    timestamp = 1;
    for (int i = 1; i <= knum; i++)
        sol[i] = round_node[i], insol[round_node[i]] = 1;
    return round_best;
    //findblock();
}

int Rand(int x) {
    int t = 0;
    for (int i = 1; i <= x; i++)
        t = t * 10 + rand() % 10;
    return t;
}

void Internal::mutation(double similar) {
    // int p = knum, l = knum * opts.mutationlen / 100;
    // for (int i = 1; i <= nnum; i++)
    //     if (!insol[i]) sol[++p] = i;
    // std::random_shuffle(sol + l + 1, sol + nnum + 1);
    // for (int i = 1; i <= nnum; i++) insol[i] = seen[i] = 0;
    // for (int i = 1; i <= knum; i++) {
    //     if (insol[sol[i]]) puts("c wrong");
    //     insol[sol[i]] = 1;
    // }
    // timestamp = 1;
    // findblock();
    // if (similar <= 0.50) return;
    double prob = pow(similar, opts.mutationpow) * opts.mutationprob; //(need div 10000)
    int limit = int(prob * 10000); //(prob * 1e8)
    for (int i = 1; i <= opts.poolsize; i++) {
        int w = Rand(8);
        if (w >= limit) continue;
        //start mutation
        for (int j = 1; j <= nnum; j++) insol[j] = 0;
        for (int j = 1; j <= knum; j++) insol[pool[i][j]] = 1, sol[j] = pool[i][j];
        random_shuffle(sol + 1, sol + knum + 1);
        int p = knum, l = knum * opts.mutationlen / 100;
        for (int j = 1; j <= nnum; j++)
            if (!insol[j]) sol[++p] = j;
        random_shuffle(sol + l + 1, sol + nnum + 1);
        for (int i = 1; i <= nnum; i++) insol[i] = seen[i] = 0;
        for (int i = 1; i <= knum; i++) {
            if (insol[sol[i]]) puts("c wrong");
            insol[sol[i]] = 1;
        }
        timestamp = 1;
        findblock();
        score[i] = improve();
        for (int j = 1; j <= knum; j++) pool[i][j] = sol[j];
        sort(pool[i] + 1, pool[i] + knum + 1);
    }
}

void Internal::simpsolve() {
    srand(time(NULL));
    auto clk_st = std::chrono::high_resolution_clock::now();
    allocmemory();
    printf("c simpsolve start\n");
    timestamp = 1;
    for (int i = 1; i <= nnum; i++) sol[i] = i, insol[i] = seen[i] = 0;
    random_shuffle(sol + 1, sol + nnum + 1);
    for (int i = 1; i <= knum; i++) insol[sol[i]] = 1;
    findblock();
//  while (std::chrono::duration_cast<std::chrono::milliseconds>(clk_now - clk_st).count() < opts.times) {
    int caltime;
    do {
        improve();
        //mutation();
        auto clk_now = std::chrono::high_resolution_clock::now();
        caltime = std::chrono::duration_cast<std::chrono::seconds>(clk_now - clk_st).count();
    } while (caltime < opts.times);
    printf("c final best val(simpsolve): %d result, find in %lu ms\n\n\n", best_val,
           std::chrono::duration_cast<std::chrono::milliseconds>(best_time - clk_st).count());
    return;
}

void Internal::cross(int x, int y) {
    for (int i = 1; i <= nnum; i++) insol[i] = seen[i] = 0;
    for (int i = 1; i <= knum; i++)
        seen[pool[x][i]]++, seen[pool[y][i]]++;
    int tail = 0;
    for (int i = 1; i <= nnum; i++) {
        if (seen[i] == 2) sol[++tail] = i, insol[i] = 1;
        if (seen[i] == 1 && rand() % 100 < opts.crossprob) sol[++tail] = i, insol[i] = 1;
    }
    timestamp = 3;
    findblock();
    if (tail < knum) {
        int pick = selectblock(), gap = knum - tail;
        ++timestamp;
        assert(block[pick].size() + tail >= knum); //high prob;
        for (int i = 1; i <= gap; i++) {
            int x = rand() % block[pick].size();
            while (seen[block[pick][x]] == timestamp) x = rand() % block[pick].size();
            int t = block[pick][x];
            seen[t] = timestamp;
            sol[++tail] = t, insol[t] = 1;
        }
        ++timestamp;
        findblock();
    } else if (tail > knum) {
        for (int i = 1; i <= tail - knum; i++)
            deletenode(tail - i + 1);
    }
}

void Internal::updatepool() {
    count++;
    for (int i = 1; i <= opts.poolsize; i++) {
        bool sameflag = 1;
        for (int j = 1; j <= knum; j++)
            if (pool[i][j] != pool[0][j]) {
                sameflag = 0;
                break;
            }
        if (sameflag) {
            if (count == 100) {
               double similar = calsimilar();
                if (similar < 0.6)
                    opts.rankpara = 30;
                else
                    opts.rankpara = 50 * similar;
                count = 0;
            }
            return;
        }
    }
    for (int i = 0; i <= opts.poolsize; i++) samenode[i] = 0;
    for (int i = 0; i <= opts.poolsize; i++) {
        ++timestamp;
        for (int j = 1; j <= knum; j++) seen[pool[i][j]] = timestamp;
        for (int j = 0; j <= opts.poolsize; j++) {
            if (i == j) continue;
            for (int k = 1; k <= knum; k++)
                if (seen[pool[j][k]] == timestamp) ++samenode[i];
        }
    }
    int mx = 0, worst = 0;
    for (int i = 0; i <= opts.poolsize; i++) {
        int diffrank = 1, scorerank = 1;
        for (int j = 0; j <= opts.poolsize; j++) {
            if (i == j) continue;
            if (samenode[j] < samenode[i]) ++diffrank;
            if (score[j] < score[i]) ++scorerank;
        }
        int total = diffrank * opts.rankpara + scorerank * (100 - opts.rankpara); //minimal is better
        if (total > mx) mx = total, worst = i;
    }

    if (worst != 0) {
        score[worst] = score[0];
        for (int i = 1; i <= knum; i++) pool[worst][i] = pool[0][i];
    }
      
    if (count == 100) {
        double similar = calsimilar();
        if (similar < 0.6)
            opts.rankpara = 30;
        else
            opts.rankpara = 50 * similar;
        count = 0;
    }

}

double Internal::calsimilar() {
    double similar = 0;
    for (int i = 1; i <= opts.poolsize; i++) {
        ++timestamp;
        int h = 0;
        for (int j = 1; j <= knum; j++) seen[pool[i][j]] = timestamp;
        for (int j = 1; j <= opts.poolsize; j++) {
            if (i == j) continue;
            for (int k = 1; k <= knum; k++)
                if (seen[pool[j][k]] == timestamp) ++h;
        }
        similar += h / (opts.poolsize - 1) / opts.poolsize;
    }
    similar /= knum;
    return similar;
}

void Internal::solve() {
    choose_best_cut = 1;
    best_val = __INT_MAX__;
    struct timeb timeSeed;
    ftime(&timeSeed);
    printf("%d\n", timeSeed.time * 1000 + timeSeed.millitm);
    srand(timeSeed.time * 1000 + timeSeed.millitm);
    auto clk_st = std::chrono::high_resolution_clock::now();
    printf("c solve start\n");
    allocmemory();

    for (int j = 1; j <= opts.poolsize; j++) {
        timestamp = 1;
        for (int i = 1; i <= nnum; i++) sol[i] = i, insol[i] = seen[i] = 0;
        random_shuffle(sol + 1, sol + nnum + 1);
        for (int i = 1; i <= knum; i++) insol[sol[i]] = 1;
        findblock();
        score[j] = improve();
        if (best_val <= opts.bestres) break;
        for (int i = 1; i <= knum; i++) pool[j][i] = sol[i];
        sort(pool[j] + 1, pool[j] + knum + 1);
        // printf("%d\n", score[j]);
    }
    printf("c finish initial, now val: %d\n", best_val);
    int caltime;

    do {
        if (best_val <= opts.bestres) break;
        int i = rand() % opts.poolsize + 1, j = rand() % opts.poolsize + 1;
        while (i == j) j = rand() % opts.poolsize + 1;
        cross(i, j);
        score[0] = improve();
        if (best_val <= opts.bestres) break;
        for (int i = 1; i <= knum; i++) pool[0][i] = sol[i];
        sort(pool[0] + 1, pool[0] + knum + 1);
        updatepool();
        double similar = calsimilar();
        auto clk_now = std::chrono::high_resolution_clock::now();
        caltime = std::chrono::duration_cast<std::chrono::seconds>(clk_now - clk_st).count();
        mutation(similar);
    } while (caltime < opts.times);

    printf("c final best val(solve): %d result, find in %lu ms\n", best_val,
           std::chrono::duration_cast<std::chrono::milliseconds>(best_time - clk_st).count());
    printf("c solution: ");
    for (int i = 1; i <= knum; i++)
        printf("%d ", best_node[i]);
    puts("");
}