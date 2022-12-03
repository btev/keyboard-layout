#ifndef __VECTOR_ADDITIONS__
#define __VECTOR_ADDITIONS__

#include<iostream>
#include<vector>
#include"global.h"
using namespace std;

// * * Removing a single value
template<class T>
vector<T> remove_index(const vector<T>& v, uint i) {
    vector<T> rtn;
    for(uint j = 0; j < v.size(); j++) {
        if (j == i)
            continue;
        rtn.push_back(v[j]);
    }
    return rtn;
}

template<class T>
vector<T> copy_without(const vector<T>& v, const T& i) {
    vector<T> rtn;
    for(uint j = 0; j < v.size(); j++) {
        if (rtn[j] == i)
            continue;
        rtn.push_back(v[j]);
    }
    return rtn;
}


// * *
template<class T>
vector<T> combine(const vector<T>& A, const vector<T>& B) {
    vector<T> AB;
    AB.reserve( A.size() + B.size() ); // allocate memory
    AB.insert( AB.end(), A.begin(), A.end() );
    AB.insert( AB.end(), B.begin(), B.end() );
    return AB;
}

template<class T>
vector<T> swap(vector<T> v, int a, int b) {
    T tmp = v[a];
    v[a] = v[b];
    v[b] = tmp;
    return v;
}

template<class T>
void print(const vector<T>& v, const string& gap = " ") {
    for(uint i = 0; i < v.size(); i++) {
        cout << v[i] << gap;
    }
    cout << endl;
}

// Inclusive
vector<int> vector_from_to(int from, int to) {
    vector<int> rtn;
    for(int i = 0; i < (to - from + 1); i++) {
        rtn.push_back(from + i);
    }
    return rtn;
}

vector<int> incrementing(int size, int start, int increment) {
    vector<int> rtn;
    for(int i = 0; i < size; i++) {
        rtn.push_back(start + (i * increment));
    }
    return rtn;
}



/*
    * * Custom super-class of vector<int> with the addition of a bitmask
        Only works with values from 0 to 31, no duplicates
*/
class v32: public vector<int> {
public:
    int mask;

    v32(): mask(0) {}

    v32(int s): vector<int>(s), mask(0) {}
    
    v32(const vector<int> v) {
        push_back(v);
    }

    void set(int i, int n) {
        int bit = 1 << n;
        if (mask & bit)
            return;
        mask |= bit;
        vector<int>::operator[](i) = n;
    }

    void remove(int i) {
        mask ^= (1 << vector<int>::operator[](i) );
        vector<int>::iterator it = vector<int>::begin();
        advance(it, i);
        vector<int>::erase(it); // dont know why they make removing a value so complicated
    }

    void push_back(int n) {
        int bit = 1 << n;
        if (mask & bit)
            return;
        mask |= bit;
        vector<int>::push_back(n);
    }

    void push_back(const vector<int>& v) {
        for(int i: v)
            push_back(i);
    }

    void pop_back() {
        mask ^= (1 << vector<int>::back());
        vector<int>::pop_back();
    }

    bool contains(int n) const {
        return mask & (1 << n);
    }

    void clear() {
        mask = 0;
        vector<int>::clear();
    }

    
    void operator+(int n) {
        push_back(n);
    }

    void operator+(const vector<int>& v) {
        push_back(v);
    }

    void operator=(const vector<int>& v) {
        vector<int>::clear();
        push_back(v);
    }

    int operator[](int i) const {
        return vector<int>::operator[](i);
    }
};

// * * mutlidimensional vectors

template<class T>
class vd: public vector<T> {
    vector<int> size;
    void allocate() {
        if (size.empty())
            return;
        
        int s = size[0];
        for(uint i = 1; i < size.size(); i++) {
            s *= size[i];
        }
        vector<T>::resize(s);
    }
public:
    //
    vd(const vector<int>& s): size(s) { allocate(); }

    T& operator[](const vector<int>& ind) {
        if (ind.size() != size.size())
            return NULL;
        int index = 1;
        for(int i = ind.size(); i >= 0; i--) {
            if (ind[i] > size[i])
                return NULL;
            index *= ind[i];
        }
        return vector<T>::operator[](index);
    }
};

// template<class T>
// class vector3D {
// public:
//     vector<vector<vector<T>>> vec;
//     vector3D(int x) {}

// };

#endif