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
//std::tr1::unordered_map<string, int> quads;

struct Two_Key {
    int speed;
    int o;
    Two_Key(): speed(0), o(0) {}
};


//
vector<vector<Two_Key>> bi_data(30, vector<Two_Key>(30));
vector<vector<vector<vector<double> > > > ALL_QUADGRAM_WEIGHTS(31, vector<vector<vector<double> > >(31, vector<vector<double> >(31, vector<double>(31))));

vector<vector<vector<int>>> ALL_TRIPLES(31, vector<vector<int>>(31, vector<int>(31, 0)));

struct TRIPLE {
    vector<int> word;
    int count;
    TRIPLE(int c): count(c) {}
    TRIPLE(const vector<int> w): word(w) {}
    TRIPLE(const vector<int> w, int c): word(w), count(c) {}
};

vector<TRIPLE> TRIPLES;

void condense() {
    for(int i = 0; i < 31; i++) {
        for(int j = 0; j < 31; j++) {
            for(int k = 0; k < 31; k++) {
                if (ALL_TRIPLES[i][j][k]) {
                    TRIPLES.push_back(ALL_TRIPLES[i][j][k]);
                    TRIPLES.back().word = {i, j, k};
                }
            }
        }
    }
}

//
void readIn() {
    ifstream myfile("filtered.txt");
    vector<int> word;
    char c;
    
    // get trip
    char tmp[3];
    myfile.read(tmp, 2);
    for(int i = 0; i < 2; i++) {
        if (!onKeyboard(c))
            word.push_back(30);
        else
            word.push_back(get_index(c));
    }

    int i = 0;
    while(myfile.get(c) && i < 10000000) { //  && i < 1000000
        //i++;
        if (!onKeyboard(c))
            word.push_back(30);
        else {
            int index = get_index(c);
            if (word[1] == index && word[2] == index)
                continue;
            singles[index]++;
            if ('A' <= c && c <= 'Z') {
                capitals[index]++;
                TOTAL_CAPITALS++;
            }
            else if (c == ':') {
                capitals[26]++;
                TOTAL_CAPITALS++;
            }
            else if (c == '<') {
                capitals[27]++;
                TOTAL_CAPITALS++;
            }
            else if (c == '>') {
                capitals[28]++;
                TOTAL_CAPITALS++;
            }
            else if (c == '?') {
                capitals[29]++;
                TOTAL_CAPITALS++;
            }
            TOTAL_CHARACTERS++;
            word.push_back(index);
        }

        ALL_TRIPLES[word[0]][word[1]][word[2]]++;
        word = remove_index(word, 0); // remove first index
    }
    condense();
}

#endif