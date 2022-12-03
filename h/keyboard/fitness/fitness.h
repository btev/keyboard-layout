#ifndef __KEYBOARD_FITNESS__
#define __KEYBOARD_FITNESS__

#include"calculation.h"
#include"weights.h"
#include"../read_in.h"

#include<iostream>
#include<vector>
using namespace std;

int get_finger(int key) {
    int column = key % 10;
    if (column >= 4)
        column--;
    if (column >= 5)
        column--;
    return column;
}

double percent_capital(const vector<int>& v) {
    double caps_pinky = 0;
    for(int i = 0; i < 30; i++) {
        if (v[i] % 10 == 0)
            caps_pinky += capitals[i];
    }
    caps_pinky /= TOTAL_CHARACTERS;
    return 1.0 + caps_pinky;
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

    double worst = 0;
    for(int i = 0; i < 8; i++) {
        fingers[i] /= TOTAL_CHARACTERS;
        double percent = fingers[i] / w.FINGER_USAGE[i]; // percentage to optimal value
        if (percent > worst) { // saving the worst value, so it should evolve with all the values close to the optimal ones
            worst = percent;
        }
    }
    rtn *= ((worst - 1) / 20) + 1;

    // left /= TOTAL_CHARACTERS;
    // rtn *= (left / 50);
    // if (49 < left && left < 51)
    //     rtn *= 0.99;
    return rtn;
}

double fitness(vector<int> v) { // in the future make this const reference
    v.push_back(30);
    // Bigrams
    double rt =  0;
    for(uint i = 0; i < TRIPLES.size(); i++) {
        rt += SCORES[v[TRIPLES[i].word[0]]][v[TRIPLES[i].word[1]]][v[TRIPLES[i].word[2]]] * TRIPLES[i].count;
    }
    //rt *= percent_hands(v);
    //rt *= percent_capital(v);
    return rt;
}

class Data {
    // * * Data
    int count;
    // Base
    vector<int> KEY_WEIGHTS = vector<int>(30, 0);
    int ALL_CAPS_PINKY = 0;
    vector<int> CAPS_PINKY = vector<int>(3, 0);

    // Bigrams
    int ALL_SAME_FINGER_BIGRAMS = 0;
    vector<int> SAME_FINGER_BIGRAM = vector<int>(5, 0);
    int DOUBLE_ALTERNATION = 0;

    // Row-Jump
    int ALL_ROW_JUMPS = 0;
    vector<int> ROW_JUMP = vector<int>(4, 0);
    int ALL_DOUBLE_ROW_JUMPS = 0;
    vector<int> DOUBLE_ROW_JUMP = vector<int>(8, 0);

    // Trigram
    vector<int> TRIGRAM_WEIGHTS = vector<int>(7, 0);
    int ALL_SAME_FINGER_TRIGRAMS = 0;
    vector<int> SAME_FINGER_TRIGRAM = vector<int>(5, 0);
    int TRIPLE_ALTERNATION = 0;

    // Overall
    vector<int> FINGER_USAGE = vector<int>(8, 0);
    vector<int> HAND_USAGE = vector<int>(2, 0);

    int CHARACTERS = 0;
    int CAPITALS = 0;
    int BIGRAMS = 0;
    int TRIGRAMS = 0;
    // * * Calculations
    vector<Letter> combo;
    int finger_1;
    int finger_2;
    int finger_3;
    // Single
    void calculate_key(const Letter& letter) { // Pair<character, position>
        CHARACTERS += count;
        KEY_WEIGHTS[letter.position] += count;
        FINGER_USAGE[letter.zero_nine_finger()] += count;
        HAND_USAGE[letter.hand] += count;
        if (!letter.capital)
            return;           // Base character weight

        CAPITALS += count;
        if (letter.position % 10)
            return;     // Base

        ALL_CAPS_PINKY += count;
        CAPS_PINKY[letter.position / 10] += count;
        return;      // Left-Pinky capital weight, 0: upper, 1: home, 2: lower
    }

    void top_bottom(const vector<Letter>& v) {
        ALL_DOUBLE_ROW_JUMPS += count;
        if (finger_1 == 1) { // If pointer
            if (v[0].row == 3) {
                DOUBLE_ROW_JUMP[0] += count;
                return;  // Not that bad, multiply as if it was a home-lower combo
            }

            if (finger_2 == 4) {
                DOUBLE_ROW_JUMP[1] += count;
                return;         // Definitely annoying
            }
        }
        else if (finger_1 == 2) {// If Middle
            if (v[0].row == 1) {
                DOUBLE_ROW_JUMP[3] += count;
                return;  // Not that bad, multiply as if it was a home-lower combo
            }

            if(finger_2 == 4) {
                DOUBLE_ROW_JUMP[4] += count;
                return;          // Pretty awkward hand spot
            }
        }
        else if (finger_2 == 1) { // If pointer
            if (v[1].row == 3) {
                DOUBLE_ROW_JUMP[0] += count;
                return;  // Not that bad, multiply as if it was a home-lower combo
            }

            if (finger_1 == 4) {
                DOUBLE_ROW_JUMP[1] += count;
                return;         // Definitely annoying
            }
        }
        else if (finger_2 == 2) {// If Middle
            if (v[1].row == 1) {
                DOUBLE_ROW_JUMP[3] += count;
                return;  // Not that bad, multiply as if it was a home-lower combo
            }

            if(finger_1 == 4) {
                DOUBLE_ROW_JUMP[4] += count;
                return;          // Pretty awkward hand spot
            }
        } // The remaining two fingers must be pinky and ring
        else if (finger_2 == 3) { // Ring
            if (v[1].row == 1) {
                DOUBLE_ROW_JUMP[6] += count;
                return;         // The last scenario I'm checking for, Lower-Pinky -> Upper-Ring
            }
        }
        else { // Pinky
            if (v[1].row == 3) {
                DOUBLE_ROW_JUMP[6] += count;
                return;
            }
        }

        DOUBLE_ROW_JUMP[7] += count;
        return; // 2.0
    }

    void calculate_bigram(const vector<Letter>& v) {
        BIGRAMS += count;

        // Alternate-Hand
        if ( v[0].hand != v[1].hand) {
            DOUBLE_ALTERNATION += count;
            return;
        }
        
        // Same-finger
        if ( (v[0].finger) == (v[1].finger) ) {
            ALL_SAME_FINGER_BIGRAMS += count;
            if ( (v[0].row ^ v[1].row) == 0) {
                if (v[0].position == v[1].position) {
                    SAME_FINGER_BIGRAM[0] += count;
                    return;
                }
                SAME_FINGER_BIGRAM[4] += count;
                return;
            }
            SAME_FINGER_BIGRAM[v[0].row ^ v[1].row] += count;
            return;
        }

        if ( !( (v[0].row ^ v[1].row) == 2) ) {
            ALL_ROW_JUMPS += count;
            ROW_JUMP[v[0].row ^ v[1].row] += count;
            return;
        }

        top_bottom(v);
        return;       // Right hand
    }

    void calculate_trigram(const vector<Letter>& v) {
        TRIGRAMS += count;
        calculate_bigram(remove_index(v, 0)); // not sure of the proper way to add these two together
        calculate_bigram(remove_index(v, 2));

        if ( v[0].hand != v[2].hand ) {
            if (v[0].hand == v[1].hand) {
                TRIGRAM_WEIGHTS[4] += count;
                return;
            }
            TRIPLE_ALTERNATION += count;
            return;
        }

        if (v[1].finger == v[2].finger) {
            return;
        }

        if (v[0].finger == v[2].finger) {
            ALL_SAME_FINGER_TRIGRAMS += count;
            if ( (v[0].row ^ v[2].row) == 0) {
                if (v[0].position == v[2].position) {
                    SAME_FINGER_TRIGRAM[0] += count;
                    return;
                }
                SAME_FINGER_TRIGRAM[4] += count;
                return;
            }
            SAME_FINGER_TRIGRAM[v[0].row ^ v[2].row] += count;
            return;
        }
        
        bool inward = (finger_1 - finger_2) > 0;

        switch(finger_1 ^ finger_2) {
            case 0:         // same-finger
                TRIGRAM_WEIGHTS[0] += count;
                return;
            case 3:         // 1 ^ 2, Pointer + Middle
                if (inward) {
                    TRIGRAM_WEIGHTS[3] += count;
                    return;
                }

                if (v[2].get_finger() == 4) {
                    TRIGRAM_WEIGHTS[1] += count;
                    return;
                }
                
                if (v[0].column == 5) {
                    TRIGRAM_WEIGHTS[2] += count;
                    return;
                }              
            
                TRIGRAM_WEIGHTS[5] += count;
                break;
            case 2:         // 1 ^ 3, Pointer + Ring
                if (v[2].get_finger() == 2) {
                    TRIGRAM_WEIGHTS[0] += count;
                    return;
                }
                if (inward) {
                    TRIGRAM_WEIGHTS[3] += count;
                    return;
                }

                if (v[2].get_finger() == 4) {
                    TRIGRAM_WEIGHTS[1] += count;
                    return;
                }
                
                if (v[0].column == 5) {
                    TRIGRAM_WEIGHTS[2] += count;
                    return;
                }            
            
                TRIGRAM_WEIGHTS[5] += count;
                return;
            case 5:         // 1 ^ 4, Pointer + Pinky
                TRIGRAM_WEIGHTS[0] += count;
                return;
            case 1:         // 2 ^ 3, Middle + Ring
                if (inward) {
                    if (v[2].get_finger() == 1) {
                        TRIGRAM_WEIGHTS[6] += count;
                        return;
                    }
                    TRIGRAM_WEIGHTS[3] += count;
                    return;
                }
                if (v[2].get_finger() == 4) {
                    TRIGRAM_WEIGHTS[1] += count;
                    return;
                }
                TRIGRAM_WEIGHTS[5] += count;
                return;
            case 6:         // 2 ^ 4, Middle, Pinky
                if (v[2].get_finger() == 3) {
                    TRIGRAM_WEIGHTS[0] += count;
                    return;
                }
                if (inward) {
                    TRIGRAM_WEIGHTS[6] += count;
                    return;
                }
                TRIGRAM_WEIGHTS[3] += count;
                return;
            default: // (7)    3 ^ 4, Ring, Pinky
                if (inward) {
                    TRIGRAM_WEIGHTS[6] += count;
                    return;
                }
                TRIGRAM_WEIGHTS[3] += count;
                return;
        }
    }
    void compute_data() {
        finger_1 = combo[0].get_finger();
        finger_2 = combo[1].get_finger();
        finger_3 = combo[2].get_finger();

        vector<Letter> v;
        for(int i = 0; i < 3; i++) {
            if (combo[i].position == 30) { // space key
                if (v.empty())
                    continue;
                if (v.size() == 1) {
                    calculate_key(v.back());
                    v.clear();
                    continue;
                }
                else {
                    calculate_key(v.front());
                    calculate_key(v.back());
                    calculate_bigram(v);
                    return;
                }
            }
            v.push_back(combo[i]);
        }

        switch(v.size()) {
            case 1:
                calculate_key(v.back()); 
                break;
            case 2:
                calculate_key(v.front());
                calculate_key(v.back());
                calculate_bigram(v);
                break;
            case 3:
                calculate_key(v[0]);
                calculate_key(v[1]);
                calculate_key(v[2]);
                calculate_trigram(v);
                break;
            default: // (0)
                break;
        }
    }
public:
    void add_data(const vector<Letter>& c, int o) {
        combo = c;
        // for(uint i = 0; i < c.size(); i++)
        //     cout << c[i].position << " ";
        // cout << endl;
        count = o;
        compute_data();
    }
    void print(int b = 50) {
        cout << "Characters" << endl;
        print_line("Total letters", b, CHARACTERS);

        print_line("Total Capitals", b, CAPITALS);
        print_line("Capitals with the left pinky", b, ALL_CAPS_PINKY);
        print_line("Capitals left-pinky top", b, CAPS_PINKY[0]);
        print_line("Capitals left-pinky home", b, CAPS_PINKY[1]);
        print_line("Capitals left-pinky bottom", b, CAPS_PINKY[2]);

        cout << "Bigrams:" << endl;
        print_line("Total Bigrams", b, BIGRAMS);
        print_line("Alternating", b, DOUBLE_ALTERNATION / (double)BIGRAMS);

        cout << endl;
        print_line("All same finger bigrams", b, ALL_SAME_FINGER_BIGRAMS / (double)BIGRAMS);
        print_line("Without same-key", b, (ALL_SAME_FINGER_BIGRAMS - SAME_FINGER_BIGRAM[0]) / (double)BIGRAMS);
        print_line("Same key", b, SAME_FINGER_BIGRAM[0] / (double)BIGRAMS);
        print_line("Same finger home->bottom", b, SAME_FINGER_BIGRAM[1] / (double)BIGRAMS);
        print_line("Same finger home->top", b, SAME_FINGER_BIGRAM[3] / (double)BIGRAMS);
        print_line("Same finger top->bottom", b, SAME_FINGER_BIGRAM[2] / (double)BIGRAMS);
        print_line("Same finger middle column", b, SAME_FINGER_BIGRAM[4] / (double)BIGRAMS);

        cout << endl;
        print_line("All row jumps", b, ALL_ROW_JUMPS / (double)BIGRAMS);
        print_line("Same row", b, ROW_JUMP[0] / (double)BIGRAMS);
        print_line("Home->bottom", b, ROW_JUMP[1] / (double)BIGRAMS);
        print_line("Home->top", b, ROW_JUMP[3] / (double)BIGRAMS);
        print_line("Top->bottom", b, ROW_JUMP[2] / (double)BIGRAMS);

        cout << endl;
        print_line("All double row jumps", b, ALL_DOUBLE_ROW_JUMPS / (double)BIGRAMS);
        print_line("Low-Pointer", b, DOUBLE_ROW_JUMP[0] / (double)BIGRAMS);
        print_line("High-Pointer -> Pinky", b, DOUBLE_ROW_JUMP[1] / (double)BIGRAMS);
        print_line("High-Pointer", b, DOUBLE_ROW_JUMP[2] / (double)BIGRAMS);
        print_line("High-Middle", b, DOUBLE_ROW_JUMP[3] / (double)BIGRAMS);
        print_line("Low-Middle -> Pinky", b, DOUBLE_ROW_JUMP[4] / (double)BIGRAMS);
        print_line("Low-Middle", b, DOUBLE_ROW_JUMP[5] / (double)BIGRAMS);
        print_line("Low-Pinky", b, DOUBLE_ROW_JUMP[6] / (double)BIGRAMS);
        print_line("Trash", b, DOUBLE_ROW_JUMP[7] / (double)BIGRAMS);

        cout << "Trigrams:" << endl;
        print_line("Total Trigrams", b, TRIGRAMS);
        print_line("Alternating", b, TRIPLE_ALTERNATION / (double)TRIGRAMS);

        cout << endl;
        print_line("All same finger trigrams", b, ALL_SAME_FINGER_TRIGRAMS / (double)TRIGRAMS);
        print_line("Without same-key", b, (ALL_SAME_FINGER_TRIGRAMS - SAME_FINGER_TRIGRAM[0]) / (double)TRIGRAMS);
        print_line("Same key", b, SAME_FINGER_TRIGRAM[0] / (double)TRIGRAMS);
        print_line("Same finger home->bottom", b, SAME_FINGER_TRIGRAM[1] / (double)TRIGRAMS);
        print_line("Same finger home->top", b, SAME_FINGER_TRIGRAM[3] / (double)TRIGRAMS);
        print_line("Same finger top->bottom", b, SAME_FINGER_TRIGRAM[2] / (double)TRIGRAMS);
        print_line("Same finger middle column", b, SAME_FINGER_TRIGRAM[4] / (double)TRIGRAMS);

        cout << endl;
        print_line("Inside-Redirect", b, TRIGRAM_WEIGHTS[0] / (double)TRIGRAMS);
        print_line("Pinky-Outward", b, TRIGRAM_WEIGHTS[1] / (double)TRIGRAMS);
        print_line("Center-Outward", b,TRIGRAM_WEIGHTS[2] / (double)TRIGRAMS);
        print_line("Redirect", b, TRIGRAM_WEIGHTS[3] / (double)TRIGRAMS);
        print_line("Roll-Alternate", b, TRIGRAM_WEIGHTS[4] / (double)TRIGRAMS);
        print_line("Full-Outward", b, TRIGRAM_WEIGHTS[5] / (double)TRIGRAMS);
        print_line("Full-Inward", b,TRIGRAM_WEIGHTS[6] / (double)TRIGRAMS);

        cout << "Hand/Fingers:" << endl;
        print_line("Left-Pinky", b, FINGER_USAGE[0] / (double)CHARACTERS);
        print_line("Left-Ring", b, FINGER_USAGE[1] / (double)CHARACTERS);
        print_line("Left-Middle", b,FINGER_USAGE[2] / (double)CHARACTERS);
        print_line("Left-Pointer", b, FINGER_USAGE[3] / (double)CHARACTERS);
        print_line("Right-Pointer", b, FINGER_USAGE[4] / (double)CHARACTERS);
        print_line("Right-Middle", b, FINGER_USAGE[5] / (double)CHARACTERS);
        print_line("Right-Ring", b,FINGER_USAGE[6] / (double)CHARACTERS);
        print_line("Right-Pinky", b,FINGER_USAGE[7] / (double)CHARACTERS);

        cout << endl;
        print_line("Left-Hand", b,HAND_USAGE[0] / (double)CHARACTERS);
        print_line("Right-Hand", b,HAND_USAGE[1] / (double)CHARACTERS);
    }
};

void print_data(vector<int> v) { // in the future make this const reference
    print_keyboard(vector_to_string(v));
    v.push_back(30);
    Data d;
    // Bigrams
    for(uint i = 0; i < TRIPLES.size(); i++)
        d.add_data({v[TRIPLES[i].word[0]], v[TRIPLES[i].word[1]], v[TRIPLES[i].word[2]]}, TRIPLES[i].count);
    d.print();
}

#endif