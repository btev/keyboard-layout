#ifndef __GENERAL_ALGORITHMS__
#define __GENERAL_ALGORITHMS__

#include<iostream>
#include<vector>
#include"general.h"
using namespace std;

// * * Iterative permutations
template<class T>
void permutation(const vector<T>& original, void (*func)(vector<T>)) {
    struct Data {
        const vector<T> v;
        int i;
        Data(vector<T> vec, int ind): v(vec), i(ind) {}
    };
    vector<Data> stack;

    // Base case
    stack.push_back({
        original,
        0
    });

    // Loop
    while (!stack.empty()) {
        Data& top = stack.back();
        if ( top.i < (int)top.v.size()) {
            top.i++;
            stack.push_back({
                remove(top.v, top.i - 1),
                0
            });
        }
        else {
            if (top.v.size() == 1) {
                vector<T> perm;
                for(Data& d: stack)
                    perm.push_back(d.v[d.i - 1]);
                func(perm);
            }
            stack.pop_back();
        }
    }
}

void permutation(const string& original, void (*func)(string)) {
    struct Data {
        const string s;
        int i;
        Data(string str, int ind): s(str), i(ind) {}
    };
    vector<Data> stack;

    // Base case
    stack.push_back({
        original,
        0
    });

    // Loop
    while (!stack.empty()) {
        Data& top = stack.back();
        if ( top.i < (int)top.s.size()) {
            top.i++;
            stack.push_back({
                top.s.substr(0, top.i - 1) + top.s.substr(top.i),
                0
            });
        }
        else {
            if (top.s.size() == 1) {
                string perm;
                for(Data& d: stack)
                    perm.push_back(d.s[d.i - 1]);
                func(perm);
            }
            stack.pop_back();
        }
    }
}

vector<int> add_to_low(const vector<int>& v, int x) {
    int total = 0;
    vector<uint> stack;

    stack.push_back(0); // Base value
    total += v[stack.back()];

    while (total != x) {
        if (total < x) {
            if (x - total >= v[0]) {
                stack.push_back(stack.back());
                total += v[stack.back()];
                continue;
            }
            if (stack.back() + 1 < v.size()) {
                total -= v[stack.back()];
                stack.back()++;
                total += v[stack.back()];
                continue;
            }
        }
        total -= v[stack.back()];
        stack.pop_back();
        while (stack.back() == v.size() - 1) {
            total -= v[stack.back()];
            stack.pop_back();
        }
        if (total == 0)
            return {};
        total -= v[stack.back()];
        stack.back()++;
        total += v[stack.back()];
    }

    vector<int> rtn;
    for(uint i = 0; i < stack.size(); i++) {
        rtn.push_back(v[stack[i]]);
    }
    return rtn;
}
vector<int> add_to_high(const vector<int>& v, int x) {
    int total = 0;
    vector<uint> stack;

    stack.push_back(v.size() - 1); // Base value
    total += v[stack.back()];

    while (total != x) {
        if (x - total >= v[0]) { // Push
            stack.push_back(stack.back());
            total += v[stack.back()];
            continue;
        }
        else if (stack.back()) { // Decr
            total -= v[stack.back()];
            stack.back()--;
            total += v[stack.back()];
            continue;
        }

        total -= v[stack.back()]; // Pop
        stack.pop_back();
        while (stack.back() == 0) {
            total -= v[stack.back()];
            stack.pop_back();
        }
        if (total == 0)
            return {};
        total -= v[stack.back()];
        stack.back()--;
        total += v[stack.back()];
    }

    vector<int> rtn;
    for(uint i = 0; i < stack.size(); i++) {
        rtn.push_back(v[stack[i]]);
    }
    return rtn;
}
vector<int> add_to_low(vector<uint> stack, int total, const vector<int>& v, int x) {
    while (total != x) {
        if (total < x) {
            if (x - total >= v[0]) {
                stack.push_back(stack.back());
                total += v[stack.back()];
                continue;
            }
            if (stack.back() + 1 < v.size()) {
                total -= v[stack.back()];
                stack.back()++;
                total += v[stack.back()];
                continue;
            }
        }
        total -= v[stack.back()];
        stack.pop_back();
        while (stack.back() == v.size() - 1) {
            total -= v[stack.back()];
            stack.pop_back();
        }
        if (total == 0)
            return {};
        total -= v[stack.back()];
        stack.back()++;
        total += v[stack.back()];
    }

    vector<int> rtn;
    for(uint i = 0; i < stack.size(); i++) {
        rtn.push_back(v[stack[i]]);
    }
    return rtn;
}
vector<int> add_to_high(vector<uint> stack, int total, const vector<int>& v, int x) {
    while (total != x) {
        if (x - total >= v[0]) { // Push
            stack.push_back(stack.back());
            total += v[stack.back()];
            continue;
        }
        else if (stack.back()) { // Decr
            total -= v[stack.back()];
            stack.back()--;
            total += v[stack.back()];
            continue;
        }

        total -= v[stack.back()]; // Pop
        stack.pop_back();
        while (stack.back() == 0) {
            total -= v[stack.back()];
            stack.pop_back();
        }
        if (total == 0)
            return {};
        total -= v[stack.back()];
        stack.back()--;
        total += v[stack.back()];
    }

    vector<int> rtn;
    for(uint i = 0; i < stack.size(); i++) {
        rtn.push_back(v[stack[i]]);
    }
    return rtn;
}

vector<int> add_to_random(const vector<int>& v, int x) {
    int total = 0;
    vector<uint> stack;

    while (total < x) { // Generating a random starting point
        stack.push_back(rand() % v.size());
        total += v[stack.back()];
    }
    
    vector<int> rtn;

    rtn = add_to_high(stack, total, v, x);
    if (rtn.empty()) {
        rtn = add_to_low(stack, total, v, x);
    }

    return rtn;
}


#endif