#ifndef __FITNESS_CALCULATIONS__
#define __FITNESS_CALCULATIONS__

#include<iostream>
#include<vector>
using namespace std;
#include"../../general/general.h"
#include"../general.h"
#include"weights.h"

Weights w;

int CHARS = 31;
vector<vector<vector<double>>> SCORES(CHARS, vector<vector<double>>(CHARS, vector<double>(CHARS)));

class Combos {
public:
    // * * Occurences
    // Base
    vector<int> KEY_WEIGHTS = vector<int>(30, 0);

    int CAPS_BASE = 0;
    vector<int> CAPS_PINKY = vector<int>(3, 0);

    // Bigrams
    vector<int> SAME_FINGER_BIGRAM = vector<int>(5, 0);
    int DOUBLE_ALTERNATION = 0;

    // Row-Jump
    vector<int> ROW_JUMP = vector<int>(4, 0);
    vector<int> DOUBLE_ROW_JUMP = vector<int>(8, 0);

    // Trigram
    vector<int> TRIGRAM_WEIGHTS = vector<int>(6, 0);
    vector<int> SAME_FINGER_TRIGRAM = vector<int>(5, 0);
    int TRIPLE_ALTERNATION = 0;

    // Overall
    vector<int> FINGER_USAGE = vector<int>(8, 0);
    vector<int> HAND_USAGE = vector<int>(2, 0);

    // * * Totals
    int CHARACTERS = 0;
    int CAPITALS = 0;
    int BIGRAMS = 0;
    int TRIGRAMS = 0;
};

Combos combos;


class Letter {
public:
    int character;
    int position;
    bool capital;
    
    int hand;
    int finger; // -4 - 4
    int row;    // 1-3
    int column;

    Letter(): character(0), position(0), capital(0) {}
    Letter(int pos): character(0), capital(0) { update_position(pos); }
    Letter(int letter, int pos, bool caps = false): character(letter), capital(caps) { update_position(pos); }
    Letter(char letter, int pos, bool caps = false): position(pos), capital(caps) {
        character = get_index(letter);
        update_position(pos);
    }

    void update_position(int pos) {
        position = pos;

        column = position % 10;
        row = (position / 10) + 1;

        // Finger
        finger = column - 4;    // -4 -3 -2 -1 0 1 2 3 4 5
        if (finger >= 2 || finger == 0)
            finger--;           // -4 -3 -2 -1  +1 +2 +3 +4


        // Hand
        hand = (column > 4);
    }

    int get_finger() const {
        return (finger > 0)? finger: -finger;
    }

    int zero_nine_finger() const {
        int rtn = finger + 4;
        if (rtn > 4)
            rtn--;
        return rtn;
    }
};


class Combo {
    vector<Letter> combo;
    double weight;
    int finger_1;
    int finger_2;
    int finger_3;
    // Single
    double calculate_key(const Letter& letter) { // Pair<character, position>
        combos.CHARACTERS++;
        if (!letter.capital)
            return w.KEY_WEIGHTS[letter.position];           // Base character weight

        combos.CAPITALS++;
        if (letter.position % 10) {
            return w.CAPS_BASE;     // Base
        }

        combos.CAPS_PINKY[letter.position / 10]++;
        return w.CAPS_PINKY[letter.position / 10];      // Left-Pinky capital weight, 0: upper, 1: home, 2: lower
    }

    double top_bottom(const vector<Letter>& v) {
        if (finger_1 == 1) { // If pointer
            if (v[0].row == 3) {
                combos.DOUBLE_ROW_JUMP[0]++;
                return w.DOUBLE_ROW_JUMP[0];  // Not that bad, multiply as if it was a home-lower combo
            }

            if (finger_2 == 4) {
                combos.DOUBLE_ROW_JUMP[1]++;
                return w.DOUBLE_ROW_JUMP[1];         // Definitely annoying
            }
        }
        else if (finger_1 == 2) {// If Middle
            if (v[0].row == 1) {
                combos.DOUBLE_ROW_JUMP[3]++;
                return w.DOUBLE_ROW_JUMP[3];  // Not that bad, multiply as if it was a home-lower combo
            }

            if(finger_2 == 4) {
                combos.DOUBLE_ROW_JUMP[4]++;
                return w.DOUBLE_ROW_JUMP[4];          // Pretty awkward hand spot
            }
        }
        else if (finger_2 == 1) { // If pointer
            if (v[1].row == 3) {
                combos.DOUBLE_ROW_JUMP[0]++;
                return w.DOUBLE_ROW_JUMP[0];  // Not that bad, multiply as if it was a home-lower combo
            }

            if (finger_1 == 4) {
                combos.DOUBLE_ROW_JUMP[1]++;
                return w.DOUBLE_ROW_JUMP[1];         // Definitely annoying
            }
        }
        else if (finger_2 == 2) {// If Middle
            if (v[1].row == 1) {
                combos.DOUBLE_ROW_JUMP[3]++;
                return w.DOUBLE_ROW_JUMP[3];  // Not that bad, multiply as if it was a home-lower combo
            }

            if(finger_1 == 4) {
                combos.DOUBLE_ROW_JUMP[4]++;
                return w.DOUBLE_ROW_JUMP[4];          // Pretty awkward hand spot
            }
        } // The remaining two fingers must be pinky and ring
        else if (finger_2 == 3) { // Ring
            if (v[1].row == 1) {
                combos.DOUBLE_ROW_JUMP[6]++;
                return w.DOUBLE_ROW_JUMP[6];         // The last scenario I'm checking for, Lower-Pinky -> Upper-Ring
            }
        }
        else { // Pinky
            if (v[1].row == 3) {
                combos.DOUBLE_ROW_JUMP[6]++;
                return w.DOUBLE_ROW_JUMP[6];
            }
        }

        combos.DOUBLE_ROW_JUMP[7]++;
        return w.DOUBLE_ROW_JUMP[7]; // 2.0
    }

    double calculate_bigram(const vector<Letter>& v) {
        combos.BIGRAMS++;
        double rtn = 1; // Adding up base letter values 

        // Alternate-Hand
        if ( v[0].hand != v[1].hand) {
            combos.DOUBLE_ALTERNATION++;
            return rtn;
        }
        
        // Same-finger
        if ( (v[0].finger) == (v[1].finger) ) {
            if ( (v[0].row ^ v[1].row) == 0) {
                if (v[0].position == v[1].position) {
                    combos.SAME_FINGER_BIGRAM[0]++;
                    return rtn * w.SAME_FINGER_BIGRAM[0];
                }
                combos.SAME_FINGER_BIGRAM[4]++;
                return rtn * w.SAME_FINGER_BIGRAM[4];
            }
            combos.SAME_FINGER_BIGRAM[v[0].row ^ v[1].row]++;
            return rtn * w.SAME_FINGER_BIGRAM[v[0].row ^ v[1].row];
        }

        if ( !( (v[0].row ^ v[1].row) == 2) ) {
            combos.ROW_JUMP[v[0].row ^ v[1].row]++;
            return rtn * w.ROW_JUMP[v[0].row ^ v[1].row];
        }

        return rtn * top_bottom(v);       // Right hand
    }

    double calculate_trigram(const vector<Letter>& v) {
        combos.TRIGRAMS++;
        // double bigram1 = (((calculate_bigram(remove_index(v, 0)) - 1)) / 2) + 1; // not sure of the proper way to add these two together
        // double bigram2 = (((calculate_bigram(remove_index(v, 2)) - 1)) / 2) + 1;
        // double rtn = bigram1 * bigram2;
        //double rtn = calculate_bigram(remove_index(v, 0)) * calculate_bigram(remove_index(v, 2));
        //double rtn = calculate_bigram(remove_index(v, 0)) + calculate_bigram(remove_index(v, 2));
        double rtn = (calculate_bigram(remove_index(v, 0)) + calculate_bigram(remove_index(v, 2))) * (0.75);

        if ( v[0].hand != v[2].hand ) {
            if (v[0].hand == v[1].hand) {
                combos.TRIGRAM_WEIGHTS[4]++;
                return rtn * w.TRIGRAM_WEIGHTS[4];        // Roll-Alternate
            }
            combos.TRIPLE_ALTERNATION++;
            return rtn;
        }

        if (v[1].finger == v[2].finger) {
            return rtn;
        }

        if (v[0].finger == v[2].finger) {
            if ( (v[0].row ^ v[2].row) == 0) {
                if (v[0].position == v[2].position) {
                    combos.SAME_FINGER_TRIGRAM[0]++;
                    return rtn * w.SAME_FINGER_TRIGRAM[0];
                }
                combos.SAME_FINGER_TRIGRAM[4]++;
                return rtn * w.SAME_FINGER_TRIGRAM[4];
            }
            combos.SAME_FINGER_TRIGRAM[v[0].row ^ v[2].row]++;
            return rtn *= w.SAME_FINGER_TRIGRAM[v[0].row ^ v[2].row];
        }

        bool inward = ((finger_1 - finger_2) > 0);

        switch(finger_1 ^ finger_2) {
            case 0:         // same-finger
                combos.TRIGRAM_WEIGHTS[0]++;
                return w.TRIGRAM_WEIGHTS[0];
            case 3:         // 1 ^ 2, Pointer + Middle
                if (inward) {
                    combos.TRIGRAM_WEIGHTS[3]++;
                    return rtn *= w.TRIGRAM_WEIGHTS[3];       // Redirect
                }

                if (v[2].get_finger() == 4) {
                    combos.TRIGRAM_WEIGHTS[1]++;
                    return rtn *= w.TRIGRAM_WEIGHTS[1];       // Pinky-Outward
                }
                
                if (v[0].column == 5) {
                    combos.TRIGRAM_WEIGHTS[2]++;
                    return rtn *= w.TRIGRAM_WEIGHTS[2];       // Center-Outward
                }              
            
                combos.TRIGRAM_WEIGHTS[5]++;
                return rtn *= w.TRIGRAM_WEIGHTS[5];           // Full-Outward
            case 2:         // 1 ^ 3, Pointer + Ring
                if (v[2].get_finger() == 2) {
                    combos.TRIGRAM_WEIGHTS[0]++;
                    return rtn *= w.TRIGRAM_WEIGHTS[0];       // Inside-Redirect
                }
                if (inward) {
                    combos.TRIGRAM_WEIGHTS[3]++;
                    return rtn *= w.TRIGRAM_WEIGHTS[3];       // Redirect
                }

                if (v[2].get_finger() == 4) {
                    combos.TRIGRAM_WEIGHTS[1]++;
                    return rtn *= w.TRIGRAM_WEIGHTS[1];       // Pinky-Outward
                }
                
                if (v[0].column == 5) {
                    combos.TRIGRAM_WEIGHTS[2]++;
                    return rtn *= w.TRIGRAM_WEIGHTS[2];       // Center-Outward
                }            
            
                combos.TRIGRAM_WEIGHTS[5]++;
                return rtn *= w.TRIGRAM_WEIGHTS[5];           // Full-Outward
            case 5:         // 1 ^ 4, Pointer + Pinky
                combos.TRIGRAM_WEIGHTS[0]++;
                return rtn *= w.TRIGRAM_WEIGHTS[0];           // Inside-Redirect
            case 1:         // 2 ^ 3, Middle + Ring
                if (inward) {
                    if (v[2].get_finger() == 1) {
                        combos.TRIGRAM_WEIGHTS[6]++;
                        return rtn *= w.TRIGRAM_WEIGHTS[6];   // Full-Inward
                    }
                    combos.TRIGRAM_WEIGHTS[3]++;
                    return rtn *= w.TRIGRAM_WEIGHTS[3];       // Redirect
                }
                if (v[2].get_finger() == 4) {
                    combos.TRIGRAM_WEIGHTS[1]++;
                    return rtn *= w.TRIGRAM_WEIGHTS[1];       // Pinky-Outward
                }
                combos.TRIGRAM_WEIGHTS[5]++;
                return rtn *= w.TRIGRAM_WEIGHTS[5];           // Full-Outward
            case 6:         // 2 ^ 4, Middle, Pinky
                if (v[2].get_finger() == 3) {
                    combos.TRIGRAM_WEIGHTS[0]++;
                    return rtn *= w.TRIGRAM_WEIGHTS[0];       // Inside-Redirect
                }
                if (inward) {
                    combos.TRIGRAM_WEIGHTS[6]++;
                    return rtn *= w.TRIGRAM_WEIGHTS[6];       // Full-Inward
                }
                combos.TRIGRAM_WEIGHTS[3]++;
                return rtn *= w.TRIGRAM_WEIGHTS[3];           // Redirect
            default: // (7)    3 ^ 4, Ring, Pinky
                if (inward) {
                    combos.TRIGRAM_WEIGHTS[6]++;
                    rtn *= w.TRIGRAM_WEIGHTS[6];              // Full-Inward
                }
                combos.TRIGRAM_WEIGHTS[3]++;
                return rtn *= w.TRIGRAM_WEIGHTS[3];           // Redirect
        }
    }
    double calculate_weight() {
        finger_1 = combo[0].get_finger();
        finger_2 = combo[1].get_finger();
        finger_3 = combo[2].get_finger();

        //
        double rtn = 0;

        vector<Letter> v;
        for(int i = 0; i < 3; i++) {
            if (combo[i].position == 30) { // space key
                if (v.empty())
                    continue;
                if (v.size() == 1) {
                    rtn += calculate_key(v.back());
                    v.clear();
                    continue;
                }
                else if (v.size() == 2) {
                    return rtn += (calculate_key(v.front()) + calculate_key(v.back()) ) * calculate_bigram(v);
                }
                v.clear();
                continue;
            }
            v.push_back(combo[i]); 
        }

        switch(v.size()) {
            case 1:
                return rtn + calculate_key(v.back());
            case 2:
                return (calculate_key(v.front()) + calculate_key(v.back()) ) * calculate_bigram(v);
            case 3:
                return (calculate_key(v[0]) + calculate_key(v[1]) + calculate_key(v[2])) * calculate_trigram(v);
            default: // (0)
                return rtn;
        }
    }
public:
    Combo(const vector<Letter>& c): combo(c) {
        weight = calculate_weight();
    }
    double get_weight() {
        return weight;
    }
};

void get_scores() {
    for(int i = 0; i < 31; i++) {
        cout << i << endl;
        for(int j = 0; j < 31; j++) {
            for(int x = 0; x < 31; x++) {
                SCORES[i][j][x] = Combo({i, j, x}).get_weight();
            }
        }
    }
}

/*
same finger trigram shouldn't include trigrams that have all three keys on the same finger
a    s    d
1.05 1.02 1

a    s    d
2.07   2.02

a    s    d
    4.07*(3/4) -> 3.0525
______________________
q    a    z
1.1  1.05 1.1

q    a    z
4.5     4.5

q    a    z
     9*(3/4) -> 6.75
______________________
q    a    z
1.1  1.05 1.1

q    a    z
2.07*2.2  2.02*2.2  -> 4.554  4.444

q    a    z
(1.05+1.02+1)*1.6*1.6 -> 7.859200000000001 11.052000000000001 the qaz the qaz was not
_____________________
l     j    k
1.02  1    1

l     j    k
2.02       2

l     j    k
(1.02+1+1)*2 -> 6.04






*/

#endif