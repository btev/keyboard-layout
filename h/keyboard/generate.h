#ifndef __GENERATE__
#define __GENERATE__


#include<iostream>
#include<vector>
#include<bits/stdc++.h> // vector sort
#include"calculator.h"
using namespace std;

#include"read_in.h"

// * * Layout
// Given a character, find it's position
class Layout: public vector<int> {
public:
    double score;
    Layout(): vector<int>(30) {}
    Layout(const vector<int>& v): vector<int>(v) {}
    Layout(const vector<int>& v, int s): vector<int>(v), score(s) {}
    Layout(const Layout& l) {
        score = l.score;
        vector<int>::operator=(l);
    }
    string to_string() {
        string rtn(30, ' ');
        for(int i = 0; i < 30; i++)
            rtn[vector<int>::operator[](i)] = get_char(i);
        return rtn;
    }
    void print(int spacing = 0) {
        cout << (int)score << endl;
        print_keyboard(vector_to_string(*this), spacing);
    }
    Layout& operator=(const Layout& l) {
        score = l.score;
        vector<int>::operator=(l);
        return *this;
    }
};

bool compare_layout(const Layout& a, const Layout& b) {
    return a.score > b.score;
}
bool compare_layout_reverse(const Layout& a, const Layout& b) {
    return a.score < b.score;
}

// All other layouts stemming from the original, with two characters swapped
vector<Layout> equivalents(Layout original, double num = 5) {
    vector<Layout> equals;
    equals.push_back(original);

    for(int i = 0; i < 30; i++) {
        for(int j = i + 1; j < 30; j++) {
            equals.push_back(swap(original, i, j));
            equals.back().score = fitness(equals.back());
        }
    }

    sort(equals.begin(), equals.end(), compare_layout_reverse);
    equals.resize(num);

    return equals;
}

// * * Swapping
vector<int> swap_two(vector<int> s) {
    int a = rand() % 30; // char
    v32& chars = LOCKS.keys[s[a]];

    int index = rand() % chars.size();
    int i = index;
    while ( !(LOCKS.chars[chars[i]].mask & (1 << s[a]))) {
        if (++i >= (int)chars.size())
            i = 0;
        if (i == index)
            return s;
    }
    int b = chars[i];

    return swap<int>(s, a, b); 
}
vector<int> swap_two(vector<int> s, int a, int b) {
    if ( !(LOCKS.chars[a].mask & (1 << b)) ) // If a cannot swap with b
        return s; 
    if ( !(LOCKS.chars[b].mask & (1 << a)) ) // b cannot swap with a
        return s; 
    return swap<int>(s, a, b);
}
// Find the best layout from all two-character swaps
Layout all_swaps(const Layout& original) {
    Layout best = original;

    for(int i = 0; i < 30; i++) { // LOCKS[] vector
        v32& chars = LOCKS.keys[original[i]];
        for(int a: chars) {
            if ( !(LOCKS.chars[i].mask & (1 << original[a])))
                continue;
            
            Layout swapped(swap(original, original[i], original[a]));
            swapped.score = fitness(swapped);
            if (swapped.score < best.score)
                best = swapped;
        }
    }

    return best;
}

vector<int> random_layout() {
    vector<int> rtn(30, -1);

    for (int i = 0; i < 30; i++) {
        v32& p = LOCKS.get_sorted(i);

        int index = rand() % p.size();
        while (rtn[p[index]] != -1) {
            index++;
            if (index >= (int)p.size())
                index = 0;
        }
        rtn[p[index]] = LOCKS.c_asc[i];
    }

    return rtn;
}

// * * Darwin and sub-functions
// Generation, generates (5 * offspring) layouts mutated from the parents and returns the five best
vector<Layout> gen(const vector<Layout> parents, int offspring) {
    vector<Layout> children;

    for(Layout l: parents) {
        children.push_back(l);
        for(int i = 0; i < offspring; i++) {
            children.push_back(swap_two(l));
            children.back().score = fitness(children.back());
        }
    }

    sort(children.begin(), children.end(), compare_layout_reverse);
    
    vector<Layout> rtn;
    rtn.push_back(children[0]);

    for(int i = 1; rtn.size() < uint(5); i++) {
        if (children[i].score == rtn.back().score)
            continue;
        rtn.push_back(children[i]);
    }

    return rtn;
}
vector<Layout> gen(int offspring = 50) {
    vector<Layout> parents;
    for(int i = 0; i < 5; i++) {
        parents.push_back(random_layout());
        parents.back().score = fitness(parents.back());
    }

    return gen(parents, offspring);
}

// Evolve, runs gen() multiple times consecutively, getting better and better layouts
vector<Layout> evolve(int generations, int offspring = 50) {
    vector<Layout> winners = gen(offspring);

    int unchanged = 0;
    int score = 0;
    for(int i = 0; i < generations; i++) {
        winners = gen(winners, offspring);
        if (winners[0].score == score)
            unchanged++;
        if (unchanged > 30)
            return winners;
    }

    return winners;
}

// Multi, runs evolve multiple times, returning the best local maximum
Layout multi(int species, int generations, int offspring = 50) {
    Layout winner = evolve(generations, offspring)[0];

    for(int i = 0; i < species; i++) {
        Layout elite = evolve(generations, offspring)[0];
        if (elite.score < winner.score) {
            winner = elite;
            winner.print();
        }
    }

    return winner;
}






// * * Main



#endif
