#ifndef __GENERAL_ALGORITHMS__
#define __GENERAL_ALGORITHMS__

#include<iostream>
#include<vector>
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


#endif