#ifndef __KEYBOARD_FITNESS__
#define __KEYBOARD_FITNESS__

#include"movements.h"
#include"weights.h"

#include<iostream>
#include<vector>
using namespace std;

/*
int get_finger(int position) {
    int column = position % 10; // 0 1 2 3 4 5 6 7 8 9
    if (column >= 4)
        column--;               // 0 1 2 3 3 4 5 6 7 8
    if (column >= 6)
        column--;               // 0 1 2 3 3 4 4 5 6 7
    return column;
    
}

double percent_hands(const vector<int>& v) {
    double rtn = 1;
    vector<double> fingers(8, 0);
    double left = 0;
    for(int i = 0; i < 30; i++) {
        fingers[get_finger(v[i])] += singles[i];
        if (i % 10 < 5)
            left +=  singles[i];
    }

    for(int i = 0; i < 8; i++) {
        fingers[i] /= TOTAL_CHARACTERS;
        if (fingers[i] < FINGER_USAGE[i] + 0.5)
            rtn *= 0.99;
    }

    left /= TOTAL_CHARACTERS;
    if (49.5 < left && left < 50.5)
        rtn *= 0.99;
    return rtn;
}

double percent_capital(const vector<int>& v) {
    double caps_pinky = 0;
    for(int i = 0; i < 30; i++) {
        if (v[i] % 10 == 0)
            caps_pinky += capitals[i];
    }
    caps_pinky /= TOTAL_CAPITALS;
    return 1.0 + caps_pinky;
}

double fitness(vector<int> v) { // in the future make this const reference
    v.push_back(30);
    // Bigrams
    double rt =  0;
    for(int i = 0; i < 31; i++) {
        for(int j = 0; j < 31; j++) {
            for(int x = 0; x < 31; x++) {
                for(int y = 0; y < 31; y++) {
                    rt += ALL_QUADGRAM_WEIGHTS[v[i]][v[j]][v[x]][v[y]] * QUADS[i][j][x][y];
                }
            }
        }
    }
    rt *= percent_hands(v);
    rt *= percent_capital(v);
    return rt;
}
*/





#endif