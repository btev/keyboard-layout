#ifndef __MISC_GENERAL__
#define __MISC_GENERAL__

#include<iostream>
#include<vector>
using namespace std;

// * * Some bitmask stuff

// works for vectors.size() <= 32
template<class T>
unsigned int bitmask(vector<T> v, bool (*func)(T)) {
    unsigned int rtn = 0;
    int bit = 1;
    for(unsigned int i = 0; i < v.size(); i++) {
        rtn |= (bit * func(v[i]));
        bit <<= 1;
    }
    return rtn;
}

// where the values in __v are less than 32
unsigned int bitmask(vector<int> v) {
    unsigned int rtn = 0;
    for(unsigned int i = 0; i < v.size(); i++) {
        rtn |= (1 << v[i]);
    }
    return rtn;
}

// where each letter of alphabet is a bit
unsigned int bitmask(string s) {
    unsigned int rtn = 0;
    for(unsigned int i = 0; i < s.size(); i++) {
        rtn |= 1 << (s[i] - 'a');
    }
    return rtn;
}





#endif