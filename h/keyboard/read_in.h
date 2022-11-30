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
v4d QUADS(31, v3d(31, v2d(31, vector<int>(31))));

void print_quads() {

}

//
void readIn() {
    ifstream myfile("filtered.txt");
    vector<int> word;
    char c;
    
    char tmp[4];
    myfile.read(tmp, 3);
    for(int i = 0; i < 3; i++) {
        if (!onKeyboard(c))
            word.push_back(30);
        else
            word.push_back(get_index(c));
    }

    int i = 0;
    while(myfile.get(c) && i < 1000000) { //  && i < 1000000
        i++;
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
            TOTAL_CHARACTERS++;
            word.push_back(index);
        }

        QUADS[word[0]][word[1]][word[2]][word[3]]++;
        word = remove_index(word, 0); // remove first index

        // if ('A' <= c && c <= 'Z') {
        //     capitals[c - 'A'] += 1;
        //     c += 32;
        // }
        // else if (c != ';' && c != '/' && c != '.' && c != ',' && !('a' <= c && c <= 'z') ) {
        //     word.clear();
        //     continue;
        // }
        
        // word.push_back(c);
        // int len = word.length();
        // switch (len) 
        // {
        // case 1:
        //     TOTAL_CHARACTERS += 1;
        //     singles[get_index(c)] += 1;
        //     break;
        // case 2:
        //     TOTAL_CHARACTERS += 1;
        //     TOTAL_BIGRAMS += 1;
        //     singles[get_index(c)] += 1;
        //     doubles[word] += 1;
        //     break;
        // case 3:
        //     TOTAL_CHARACTERS += 1;
        //     TOTAL_BIGRAMS += 1;
        //     TOTAL_TRIGRAMS += 1;
        //     singles[get_index(c)] += 1;
        //     doubles[word.substr(1)] += 1;
        //     triples[word] += 1;
        //     break;
        // default: // 4 or greater
        //     TOTAL_CHARACTERS += 1;
        //     TOTAL_BIGRAMS += 1;
        //     TOTAL_TRIGRAMS += 1;
        //     TOTAL_QUADGRAMS += 1;
        //     quads[word.substr(len - 4)] += 1;
        //     break;
        // };
    }
}

#endif