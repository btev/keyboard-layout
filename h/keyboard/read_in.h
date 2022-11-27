#ifndef __READ_IN__
#define __READ_IN__


#include<iostream>
#include<fstream>
#include<tr1/unordered_map>
using namespace std;

#include"../general/general.h"
#include"general.h"
#include"locks.h"

Locks LOCKS;

int TOTAL_CAPITALS = 0;
int TOTAL_CHARACTERS = 0;
int TOTAL_BIGRAMS = 0;
int TOTAL_TRIGRAMS = 0;
int TOTAL_QUADGRAMS = 0;
vector<int> capitals(30, 0);
vector<int> singles(30, 0);
std::tr1::unordered_map<string, int> doubles;
std::tr1::unordered_map<string, int> triples;
std::tr1::unordered_map<string, int> quads;

vector<double> k_weights{
     1.1, 1.07, 1.05, 1.05, 1.07,  1.2, 1.05, 1.05, 1.07,  1.1,
    1.05, 1.02,    1,    1,  1.1,  1.1,    1,    1, 1.02, 1.05,
     1.1,  1.1, 1.05, 1.05,  1.2,  1.1, 1.05, 1.05,  1.1,  1.1
};
vector<vector<double>> bi_weights(30, vector<double>(30));

double get_weight(int x, int y) {
    Key a(x);
    Key b(y);

    double rtn = k_weights[x] + k_weights[y]; // Base position value (not sure whether to multiply or add)

        // If same-hand
    if (a.hand == b.hand) { // If same-hand
        // Same-finger
        if ( (a.finger) == (b.finger) )
            rtn *= 2;
        
        // Which hand
        if (!a.hand) { // left

            // Upper-lower
            if ((a.row | b.row) == 2) {
                rtn *= 1.5;
            } // Inward-Roll
            else if (b.finger > a.finger)
                rtn *= 0.9; // 0.7
        }
        else { // right
            // Upper-lower
            if ((a.row | b.row) == 2) {
                rtn *= 1.5;
            } // Inward-Roll
            else if (b.finger < a.finger) 
                rtn *= 0.9; // 0.7
        }
    }
    else {
        // Alternate
        rtn *= 0.9; // 0.9
    }

    return rtn;
}

//
void readIn() {
    ifstream myfile("filtered.txt");
    string word = "";
    char c;

    int i = 0;
    while(myfile.get(c) && i < 1000000) { //  && i < 1000000
        i++;
        if ('A' <= c && c <= 'Z') {
            capitals[c - 'A'] += 1;
            c += 32;
        }
        else if (c != ';' && c != '/' && c != '.' && c != ',' && !('a' <= c && c <= 'z') ) {
            word.clear();
            continue;
        }
        
        word.push_back(c);
        int len = word.length();
        switch (len) 
        {
        case 1:
            TOTAL_CHARACTERS += 1;
            singles[get_index(c)] += 1;
            break;
        case 2:
            TOTAL_CHARACTERS += 1;
            TOTAL_BIGRAMS += 1;
            singles[get_index(c)] += 1;
            doubles[word] += 1;
            break;
        case 3:
            TOTAL_CHARACTERS += 1;
            TOTAL_BIGRAMS += 1;
            TOTAL_TRIGRAMS += 1;
            singles[get_index(c)] += 1;
            doubles[word.substr(1)] += 1;
            triples[word] += 1;
            break;
        default: // 4 or greater
            TOTAL_CHARACTERS += 1;
            TOTAL_BIGRAMS += 1;
            TOTAL_TRIGRAMS += 1;
            TOTAL_QUADGRAMS += 1;
            singles[get_index(c)] += 1;
            doubles[word.substr(len - 2)] += 1;
            triples[word.substr(len - 3)] += 1;
            quads[word.substr(len - 4)] += 1;
            break;
        };
    }

    // Weights
    for(int i = 0; i < 30; i++) {
        for(int j = 0; j < 30; j++) {
            bi_weights[i][j] = get_weight(i, j);
        }
    }
}

#endif