#ifndef __KEYBOARD_DISPLAY__
#define __KEYBOARD_DISPLAY__

#include<iostream>
#include<vector>
using namespace std;

#include"read_in.h"

// General key weights just based on their position on the keyboard
vector<double> KEY_WEIGHTS{
     1.1, 1.07, 1.05, 1.05, 1.07,  1.2, 1.05, 1.05, 1.07,  1.1,
    1.05, 1.02,    1,    1,  1.1,  1.1,    1,    1, 1.02, 1.05,
     1.1,  1.1, 1.05, 1.05,  1.2,  1.1, 1.05, 1.05,  1.1,  1.1
};

// General key weights for having to hold shift and hit a letter in the pinky column
vector<double> CAPS_PINKY = {
    1.7,
    1.6,
    1.75
};

// Using the same finger at the beginning and the end of a sequence. ex: "ki", "cod", "come"
vector<double> SAME_FINGER_BIGRAM = {
    1,    // Same-key 1.4
    2.2,    // Home-Lower
    3,      // Upper-Lower
    2.2     // Home-Upper
};
vector<double> SAME_FINGER_BIGRAM_COUNT = {
    0,
    0,
    0,
    0
};

vector<double> SAME_FINGER_TRIGRAM = {
    1,
    1.4,
    2.2,
    1.4
};
vector<double> SAME_FINGER_TRIGRAM_COUNT = {
    0,
    0,
    0,
    0
};
vector<double> SAME_FINGER_QUADGRAM = {
    1,
    1,
    1.4,
    1,
};
vector<double> SAME_FINGER_QUADGRAM_COUNT = {
    1,
    1,
    1.4,
    1,
};

// Having to type a key from a different row on the same hand
vector<double> ROW_JUMP = {
    1,    // Same-Row
    1.1,   // Home-Lower
    1,      // placeholder
    1.05   // Home-Upper
};
vector<double> ROW_JUMP_COUNT = {
    0,
    0,
    0,
    0
};

vector<double> DOUBLE_ROW_JUMP = {
    1,      // Low-Pointer
    1.25,   // High-Pointer -> Pinky
    2,      // High-Pointer

    1,      // High-Middle
    1.5,    // Low-Middle -> Pinky
    2,      // Low-Middle

    1.25,   // Low-Pinky
    2       // Low-Ring
};
vector<double> DOUBLE_ROW_JUMP_COUNT = {
    0,
    0,
    0,

    0,
    0,
    0,

    0,
    0,
    0,
};

// General triple scenarios and the weights associated
vector<double> TRIGRAM_WEIGHTS = {
    2,      // Inside-Redirect: The first two keys roll with a gap in-between, where the final letter is located. Ex: "das", "ljk"
    1.5,    // Pinky-Outward:   An outward roll that contains the pinky
    1.3,    // Center-Outward:  An outward roll with the pointer on the center column
    1.2,    // Redirect:        Rolling in a direction and then having to swap direction (same-hand)
    0.8,    // Roll-Alternate:  The first two keys roll and the last one alternates
    0.8,    // Full-Outward:    An outward roll that doesn't belong to one of the categories mentioned above
    0.65    // Full-Inward:     An inward roll
};
vector<double> TRIGRAM_WEIGHTS_COUNT = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};

vector<double> QUADGRAM_WEIGHTS = {
    0.8,    // Full Outward: A four character outward-roll
    0.85,   // Roll-Alternate: A triple roll and the final key swaps hands
    0.8     // Full-Inward: A full four character inward roll (note this is on top of the triple 0.65 multiplier, so really it's 0.52x)
};
vector<double> QUADGRAM_WEIGHTS_COUNT = {
    0,    // Full Outward: A four character outward-roll
    0,   // Roll-Alternate: A triple roll and the final key swaps hands
    0     // Full-Inward: A full four character inward roll (note this is on top of the triple 0.65 multiplier, so really it's 0.52x)
};

// Overall Bonuses (Note these are just the desired percentages in each category, for every category the layout hits it's entire fitness it multiplied by USAGE_MULIPLIER, 0.99x )
vector<double> FINGER_USAGE = {
    8,
    11,
    16,
    15
};

double ALTERNATION = 0;

double HAND_USAGE = 50;

double USAGE_MULTIPLIER = 0.99;

// Weight Calculations
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
    print_buffer("LEFT_PINKY: " + to_string(fingers[0]), 30);
    print_buffer("LEFT_RING: " + to_string(fingers[1]), 30);
    print_buffer("LEFT_MIDDLE: " + to_string(fingers[2]), 30);
    print_buffer("LEFT_POINTER: " + to_string(fingers[3]), 30);
    print_buffer("RIGHT_PINKY: " + to_string(fingers[4]), 30);
    print_buffer("RIGHT_RING: " + to_string(fingers[5]), 30);
    print_buffer("RIGHT_MIDDLE: " + to_string(fingers[6]), 30);
    print_buffer("RIGHT_POINTER: " + to_string(fingers[7]), 30);

    left /= TOTAL_CHARACTERS;
    print_buffer("LEFT_HAND: " + to_string(left), 30);
    print_buffer("RIGHT_HAND: " + to_string(1 - left), 30);
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
    print_buffer("CAPS_PINKY: " + to_string(caps_pinky), 30);
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
// double fitness(const vector<int>& v) {
//     // Bigrams
//     for(std::tr1::unordered_map<vector<int>, int>::const_iterator it = QUADS.begin(); it != QUADS.end(); ++it){
//         rt += ALL_QUADGRAM_WEIGHTS[v[it->first[0]]][v[it->first[1]]][v[it->first[2]]][v[it->first[3]]]* it->second; // this line lol
//     }

//     return rt;
// }

// double get_weight(int x, int y) {
//     Key a(x);
//     Key b(y);

//     double rtn = k_weights[x] + k_weights[y]; // Base position value (not sure whether to multiply or add)

//         // If same-hand
//     if (a.hand == b.hand) { // If same-hand
//         // Same-finger
//         if ( (a.finger) == (b.finger) )
//             rtn *= 2;
        
//         // Which hand
//         if (!a.hand) { // left

//             // Upper-lower
//             if ((a.row | b.row) == 2) {
//                 rtn *= 1.5;
//             } // Inward-Roll
//             else if (b.finger > a.finger)
//                 rtn *= 0.9; // 0.7
//         }
//         else { // right
//             // Upper-lower
//             if ((a.row | b.row) == 2) {
//                 rtn *= 1.5;
//             } // Inward-Roll
//             else if (b.finger < a.finger) 
//                 rtn *= 0.9; // 0.7
//         }
//     }
//     else {
//         // Alternate
//         rtn *= 0.9; // 0.9
//     }

//     return rtn;
// }

// could use the 256 (1 << 8), as the capitalization bit. That way when casting to a char, it would remove the bit leaving the 0-29 value. Otherwise a negative value could be capital as-well
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
};


double calculate_key(const Letter& letter) { // Pair<character, position>
    // if (!letter.capital)
    //     return KEY_WEIGHTS[letter.position];           // Base character weight
    
    // if (letter.position % 10)
    //     return CAPS_PINKY[3];              // Base capital weight
    
    // return CAPS_PINKY[letter.position / 10];      // Left-Pinky capital weight, 0: upper, 1: home, 2: lower
    return KEY_WEIGHTS[letter.position];
}

/*
This whole next section is pretty ugly, but all it's doing is just checking for these scenarios
### Double row jump (same hand)
Pointer: 
Pointer is lower       : Good   1x
Pointer-Higher-> pinky : Ok     1.25x
Pointer is higher      : Trash  2x

Middle:
Middle is upper        : Good   1x
Middle-Lower->Pinky    : Ok     1.5x
Middle is lower        : Trash  2x

Ring -> Pinky:
Lower Pinky            : Ok     1.25x
Lower Ring             : Trash  2x
*/

/*
    1,      // Low-Pointer
    1.25,   // High-Pointer -> Pinky
    2,      // High-Pointer

    1,      // High-Middle
    1.5,    // Low-Middle -> Pinky
    2,      // Low-Middle

    1.25,   // Low-Pinky
    2       // Low-Ring
*/
double top_bottom(const vector<Letter>& v, int o) {
    int a = v[0].get_finger();
    int b = v[1].get_finger();

    if (a == 1) { // If pointer
        if (v[0].row == 3) {// Lower
            DOUBLE_ROW_JUMP_COUNT[0] += o;
            return ROW_JUMP[1];  // Not that bad, multiply as if it was a home-lower combo
        }

        if (b == 4) {// If pinky
            DOUBLE_ROW_JUMP_COUNT[1] += o;
            return 1.25;         // Definitely annoying
        }

    }
    else if (a == 2) {// If Middle
        if (v[0].row == 1) {// Upper
            DOUBLE_ROW_JUMP_COUNT[3] += o;
            return ROW_JUMP[1];  // Not that bad, multiply as if it was a home-lower combo
        }

        if(b == 4) {// If pinky
            DOUBLE_ROW_JUMP_COUNT[4] += o;
            return 1.5;          // Pretty awkward hand spot
        }
    }
    else if (b == 1) { // If pointer
        if (v[1].row == 3) {// Lower
            DOUBLE_ROW_JUMP_COUNT[0] += o;
            return ROW_JUMP[1];  // Not that bad, multiply as if it was a home-lower combo
        }
        if (a == 4) {// If pinky
            DOUBLE_ROW_JUMP_COUNT[1] += o;
            return 1.25;         // Definitely annoying
        }
    }
    else if (b == 2) {// If Middle
        if (v[1].row == 1) {// Upper
            DOUBLE_ROW_JUMP_COUNT[3] += o;
            return ROW_JUMP[1];  // Not that bad, multiply as if it was a home-lower combo
        }
        if(a == 4) {// If pinky
            DOUBLE_ROW_JUMP_COUNT[4] += o;
            return 1.5;          // Pretty awkward hand spot
        }
    } // The remaining two fingers must be pinky and ring
    else if (b == 3) { // Ring
        if (v[1].row == 1) {
            DOUBLE_ROW_JUMP_COUNT[6] += o;
            return 1.25;         // The last scenario I'm checking for, Lower-Pinky -> Upper-Ring
        }
    }
    else { // Pinky
        if (v[1].row == 3) {
            DOUBLE_ROW_JUMP_COUNT[6] += o;
            return 1.25;
        }
    }
    return 2.0;
}

double calculate_bigram(const vector<Letter>& v, int o) {
    TOTAL_BIGRAMS += o;
    double rtn = 0; // Adding up base letter values 

    // Alternate-Hand
    if ( v[0].hand != v[1].hand) {
        ALTERNATION += o;
        return rtn;
    }
    
    // Same-finger
    if ( (v[0].finger) == (v[1].finger) ) {
        SAME_FINGER_BIGRAM_COUNT[v[0].row ^ v[1].row] += o;
        return rtn * SAME_FINGER_BIGRAM[v[0].row ^ v[1].row];
    }

    if ( !(v[0].row + v[1].row == 4) ) {
        ROW_JUMP_COUNT[v[0].row ^ v[1].row];
        return rtn * ROW_JUMP[v[0].row ^ v[1].row];
    }

    if (!v[0].hand)
        return rtn * top_bottom(v, o);    // Left hand

    return rtn * top_bottom(v, o);       // Right hand
}

double calculate_trigram(const vector<Letter>& v, int o) {
    TOTAL_TRIGRAMS += o;
    double rtn = 0;

    if ( v[0].hand != v[2].hand ) {
        if (v[0].hand == v[1].hand) {
            TRIGRAM_WEIGHTS_COUNT[4] += o;
            return rtn * TRIGRAM_WEIGHTS[4];        // Roll-Alternate
        }
        return rtn;
    }

    if (v[0].finger == v[2].finger){
        SAME_FINGER_TRIGRAM_COUNT[v[0].row ^ v[2].row] += o;
        return rtn *= SAME_FINGER_TRIGRAM[v[0].row ^ v[2].row];
    }

    int finger_1 = v[0].get_finger();
    int finger_2 = v[1].get_finger();
    bool inward = (finger_1 - finger_2) > 0;

    switch(finger_1 ^ finger_2) {
        case 0:         // same-finger
            return rtn;
        case 3:         // 1 ^ 2, Pointer + Middle
            if (inward) {
                TRIGRAM_WEIGHTS_COUNT[3] += o;
                return rtn *= TRIGRAM_WEIGHTS[3];       // Redirect
            }

            if (v[2].get_finger() == 4) {// pinky 
                TRIGRAM_WEIGHTS_COUNT[1] += o;
                return rtn *= TRIGRAM_WEIGHTS[1];       // Pinky-Outward
            }
            
            if (v[0].column == 5)            {
                TRIGRAM_WEIGHTS[2] += o;
                return rtn *= TRIGRAM_WEIGHTS[2];       // Center-Outward
            }            
        
            TRIGRAM_WEIGHTS_COUNT[5] += o;
            return rtn *= TRIGRAM_WEIGHTS[5];           // Full-Outward
        case 2:         // 1 ^ 3, Pointer + Ring
            if (v[2].get_finger() == 2) {
                TRIGRAM_WEIGHTS_COUNT[0] += o;
                return rtn *= TRIGRAM_WEIGHTS[0];       // Inside-Redirect
            }
            if (inward) {
                TRIGRAM_WEIGHTS_COUNT[3] += o;
                return rtn *= TRIGRAM_WEIGHTS[3];       // Redirect
            }

            if (v[2].get_finger() == 4) {// pinky
                TRIGRAM_WEIGHTS_COUNT[1] += o;
                return rtn *= TRIGRAM_WEIGHTS[1];       // Pinky-Outward
            }
            
            if (v[0].column == 5)                        {
                TRIGRAM_WEIGHTS_COUNT[2] += o;
                return rtn *= TRIGRAM_WEIGHTS[2];       // Center-Outward
            }
            TRIGRAM_WEIGHTS_COUNT[5] += o;
            return rtn *= TRIGRAM_WEIGHTS[5];           // Full-Outward
        case 5:         {// 1 ^ 4, Pointer + Pinky {
            TRIGRAM_WEIGHTS_COUNT[0] += o;
            return rtn *= TRIGRAM_WEIGHTS[0];           // Inside-Redirect
        }
        case 1:         // 2 ^ 3, Middle + Ring
            if (inward) {
                if (v[2].get_finger() == 1) {
                    TRIGRAM_WEIGHTS_COUNT[6] += o;
                    return rtn *= TRIGRAM_WEIGHTS[6];   // Full-Inward
                }
                TRIGRAM_WEIGHTS_COUNT[3] += o;
                return rtn *= TRIGRAM_WEIGHTS[3];       // Redirect
            }
            if (v[2].get_finger() == 4) {
                TRIGRAM_WEIGHTS_COUNT[1] += o;
                return rtn *= TRIGRAM_WEIGHTS[1];       // Pinky-Outward
            }
            TRIGRAM_WEIGHTS_COUNT[5] += o;
            return rtn *= TRIGRAM_WEIGHTS[5];           // Full-Outward
        case 6:         // 2 ^ 4, Middle, Pinky
            if (v[2].get_finger() == 3) {
                TRIGRAM_WEIGHTS_COUNT[0] += o;
                return rtn *= TRIGRAM_WEIGHTS[0];       // Inside-Redirect
            }
            if (inward) {
                TRIGRAM_WEIGHTS_COUNT[6] += o;
                return rtn *= TRIGRAM_WEIGHTS[6];       // Full-Inward
            }
            TRIGRAM_WEIGHTS_COUNT[3] += o;
            return rtn *= TRIGRAM_WEIGHTS[3];           // Redirect
        default: // (7)    3 ^ 4, Ring, Pinky
            if (inward) {
                TRIGRAM_WEIGHTS_COUNT[6] += o;
                rtn *= TRIGRAM_WEIGHTS[6];              // Full-Inward
            }
            TRIGRAM_WEIGHTS_COUNT[3] += o;
            return rtn *= TRIGRAM_WEIGHTS[3];           // Redirect
    }
}
double calculate_quadgram(const vector<Letter>& v, int o) {
    TOTAL_QUADGRAMS += o;
    double rtn = 0;

    if ( v[0].hand != v[2].hand ) {
        if ((v[0].hand == v[1].hand) && (v[1].hand == v[2].hand)) {
            QUADGRAM_WEIGHTS_COUNT[1] += o;
            return rtn * QUADGRAM_WEIGHTS[1];            // Roll-Alternate
        }
        return rtn;
    }

    if (v[0].finger == v[3].finger) {
        SAME_FINGER_QUADGRAM_COUNT[v[0].row ^ v[2].row] += o;
        return rtn *= SAME_FINGER_QUADGRAM[v[0].row ^ v[2].row];
    }

    int finger_1 = v[0].get_finger();
    int finger_2 = v[1].get_finger();
    int finger_3 = v[2].get_finger();

    if ( (finger_2 ^ finger_3) == 1) { // Middle + Ring in the middle is needed for a roll
        if (finger_2 == 2) { // Potential outward roll
            if (finger_1 == 1 && v[3].get_finger() == 4) {
                QUADGRAM_WEIGHTS_COUNT[0] += o;
                return rtn *= QUADGRAM_WEIGHTS[0];          // Full-Outward
            }
        }
        else {               // Potential inward roll
            if (finger_2 == 4 && v[3].get_finger() == 1) {
                QUADGRAM_WEIGHTS_COUNT[2] += o;
                return rtn *= QUADGRAM_WEIGHTS[2];          // Full-Inward
            }
        }

    }

    return rtn;
}

// 923521 combos btw
double get_weight(const vector<Letter>& quad, int o) {
    double rtn = 0;

    vector<Letter> v;
    for(int i = 0; i < 4; i++) {
        if (quad[i].position == 30) {
            switch(v.size()) {
                case 1:
                    rtn += calculate_key(v.back().character);
                    break;
                case 2:
                    rtn += calculate_bigram(v, o);
                    break;
                case 3:
                    calculate_bigram(remove_index(v, 0), o);
                    calculate_bigram(remove_index(v, 2), o);
                    return rtn + calculate_trigram(v, o);
                default: // (0)
                    continue;
            }
            v.clear();
        }
        v.push_back(quad[i]);
    }

    switch(v.size()) {
        case 1:
            return rtn + calculate_key(v.back().character);
            break;
        case 2:
            return rtn + calculate_bigram(v, o);
            break;
        case 3:
            calculate_bigram(remove_index(v, 0), o);
            calculate_bigram(remove_index(v, 2), o);
            return calculate_trigram(v, o);
        case 4:
            calculate_bigram({v[0],v[1]}, o);
            calculate_bigram({v[1],v[2]}, o);
            calculate_bigram({v[2],v[3]}, o);
            calculate_trigram(remove_index(v, 0), o);
            calculate_trigram(remove_index(v, 3), o);
            return calculate_quadgram(v, o);
        default: // (0)
            return rtn;
    }
}
void print_all_data() {
    cout << endl << "SAME_FINGER_BIGRAM" << endl;
    print_buffer("SAME_KEY: " +  to_string(SAME_FINGER_BIGRAM_COUNT[0] / TOTAL_BIGRAMS), 30);
    print_buffer("HOME_LOWER: " +  to_string(SAME_FINGER_BIGRAM_COUNT[1] / TOTAL_BIGRAMS), 30);
    print_buffer("UPPER_LOWER: " +  to_string(SAME_FINGER_BIGRAM_COUNT[2] / TOTAL_BIGRAMS), 30);
    print_buffer("HOME_UPPER: " +  to_string(SAME_FINGER_BIGRAM_COUNT[3] / TOTAL_BIGRAMS), 30);

    cout << endl << "ROW_JUMP" << endl;
    print_buffer("SAME_ROW: " +  to_string(ROW_JUMP_COUNT[0] / TOTAL_BIGRAMS), 30);
    print_buffer("HOME_LOWER: " +  to_string(ROW_JUMP_COUNT[1] / TOTAL_BIGRAMS), 30);
    print_buffer("UPPER_LOWER: " +  to_string(ROW_JUMP_COUNT[2] / TOTAL_BIGRAMS), 30);
    print_buffer("HOME_UPPER: " +  to_string(ROW_JUMP_COUNT[3] / TOTAL_BIGRAMS), 30);

    cout << endl << "DOUBLE_ROW_JUMP" << endl;
    print_buffer("Inside-Redirect: " +  to_string(DOUBLE_ROW_JUMP_COUNT[0] / TOTAL_BIGRAMS), 30);
    print_buffer("Pinky-Outward: " +  to_string(DOUBLE_ROW_JUMP_COUNT[1] / TOTAL_BIGRAMS), 30);
    print_buffer("Center-Outward: " +  to_string(DOUBLE_ROW_JUMP_COUNT[2] / TOTAL_BIGRAMS), 30);
    print_buffer("Redirect-: " +  to_string(DOUBLE_ROW_JUMP_COUNT[3] / TOTAL_BIGRAMS), 30);
    print_buffer("Roll-Alternate: " +  to_string(DOUBLE_ROW_JUMP_COUNT[4] / TOTAL_BIGRAMS), 30);
    print_buffer("Full-Outward: " +  to_string(DOUBLE_ROW_JUMP_COUNT[5] / TOTAL_BIGRAMS), 30);
    print_buffer("Full-Inward: " +  to_string(DOUBLE_ROW_JUMP_COUNT[6] / TOTAL_BIGRAMS), 30);


    cout << endl << "TRIGRAM_SAME_FINGER" << endl;
    print_buffer("SAME_KEY: " +  to_string(SAME_FINGER_TRIGRAM_COUNT[0] / TOTAL_TRIGRAMS), 30);
    print_buffer("HOME_LOWER: " +  to_string(SAME_FINGER_TRIGRAM_COUNT[1] / TOTAL_TRIGRAMS), 30);
    print_buffer("UPPER_LOWER: " +  to_string(SAME_FINGER_TRIGRAM_COUNT[2] / TOTAL_TRIGRAMS), 30);
    print_buffer("HOME_UPPER: " +  to_string(SAME_FINGER_TRIGRAM_COUNT[3] / TOTAL_TRIGRAMS), 30);


    cout << endl << "TRIGRAM_WEIGHTS" << endl;
    print_buffer("Inside-Redirect: " +  to_string(TRIGRAM_WEIGHTS_COUNT[0] / TOTAL_TRIGRAMS), 30);
    print_buffer("Pinky-Outward: " +  to_string(TRIGRAM_WEIGHTS_COUNT[1] / TOTAL_TRIGRAMS), 30);
    print_buffer("Center-Outward: " +  to_string(TRIGRAM_WEIGHTS_COUNT[2] / TOTAL_TRIGRAMS), 30);
    print_buffer("Redirect: " +  to_string(TRIGRAM_WEIGHTS_COUNT[3] / TOTAL_TRIGRAMS), 30);
    print_buffer("Roll-Alternate: " +  to_string(TRIGRAM_WEIGHTS_COUNT[4] / TOTAL_TRIGRAMS), 30);
    print_buffer("Full-Outward: " +  to_string(TRIGRAM_WEIGHTS_COUNT[5] / TOTAL_TRIGRAMS), 30);
    print_buffer("Full-Inward: " +  to_string(TRIGRAM_WEIGHTS_COUNT[6] / TOTAL_TRIGRAMS), 30);


    cout << endl << "QUADGRAM_SAME_FINGER" << endl;
    print_buffer("SAME_KEY: " +  to_string(SAME_FINGER_QUADGRAM_COUNT[0] / TOTAL_QUADGRAMS), 30);
    print_buffer("HOME_LOWER: " +  to_string(SAME_FINGER_QUADGRAM_COUNT[1] / TOTAL_QUADGRAMS), 30);
    print_buffer("UPPER_LOWER: " +  to_string(SAME_FINGER_QUADGRAM_COUNT[2] / TOTAL_QUADGRAMS), 30);
    print_buffer("HOME_UPPER: " +  to_string(SAME_FINGER_QUADGRAM_COUNT[3] / TOTAL_QUADGRAMS), 30);

    cout << endl << "TRIGRAM_WEIGHTS" << endl;
    print_buffer("Full Outward: " +  to_string(QUADGRAM_WEIGHTS_COUNT[0] / TOTAL_QUADGRAMS), 30);
    print_buffer("Roll-Alternate: " +  to_string(QUADGRAM_WEIGHTS_COUNT[1] / TOTAL_QUADGRAMS), 30);
    print_buffer("Full-Inward: " +  to_string(QUADGRAM_WEIGHTS_COUNT[2] / TOTAL_QUADGRAMS), 30);
}


void anecdotal_weights(vector<int> v) {
    print_keyboard(vector_to_string(v));
    v.push_back(30);
    // Weights
    for(int i = 0; i < 31; i++) {
        for(int j = 0; j < 31; j++) {
            for(int x = 0; x < 31; x++) {
                for(int y = 0; y < 31; y++) {
                    get_weight({v[i], v[j], v[x], v[y]}, QUADS[i][j][x][y]);
                    // if (ALL_QUADGRAM_WEIGHTS[i][j][x][y] > 15) {
                    //     cout << i << " " << j << " " <<  x << " " << y << endl;
                    // }
                }
            }
        }
    }

    print_all_data();
}


// Keyboard stats
class Keyboard {
    void print_line(string title, double a) {
        print_buffer(title, 22);
        cout << ":   ";
        print_buffer(to_string(a), 15);
        cout << endl;
    }

    bool add_double(const Key& a, const Key& b, int o) {
        bool inward = false;
        if (a.hand == b.hand) { // If same-hand
            // Same-finger
            if ( (a.finger) == (b.finger) )
                double_same += o;
            
            // Which hand
            if (!a.hand) { // left

                // Upper-lower
                if ((a.row | b.row) == 2) {
                    upperlower += o;
                } // Inward-Roll
                else if (b.finger > a.finger) {
                    double_inward += o;
                    inward = true;
                }
            }
            else { // right
                // Upper-lower
                if ((a.row | b.row) == 2) {
                    upperlower += o;
                } // Inward-Roll
                else if (b.finger < a.finger)  {
                    double_inward += o;
                    inward = true;
                }
            }
        }
        else {
            // Alternate
            alternation += o;
        }
        return inward;
    }

    void add_stat(string s, int o) {
        Key a(s.front());
        Key b(s[1]);
        Key c(s[2]);
        Key d(s.back());

        // Doubles
        bool inward = add_double(a, b, o);

        // Triples
        if ( (a.finger) == (c.finger) )
            triple_same += o;
        else if(inward) {
            if (b.hand == c.hand) {
                if (!b.hand) {
                    if (c.finger > b.finger)
                        triple_inward += o;
                }
                else if (c.finger < b.finger){
                    triple_inward += o;
                }
            }
            else
                inward = false;
        }

        // Quads
        if ( (a.finger) == (d.finger) )
            triple_same += o;
        else if(inward) {
            if (c.hand == d.hand) {
                if (!c.hand) {
                    if (d.finger > c.finger)
                        double_inward += o;
                }
                else if (d.finger < c.finger)
                    double_inward += o;
            }
            else {
                inward = false;
            }
        }
    }

    // 0 = bigram, 1 
    void div_total() {
        double_inward /= TOTAL_BIGRAMS;
        triple_inward /= TOTAL_TRIGRAMS;
        quad_inward /= TOTAL_QUADGRAMS;

        double_inward /= TOTAL_BIGRAMS;
        triple_inward /= TOTAL_TRIGRAMS;
        quad_inward /= TOTAL_QUADGRAMS;

        alternation /= TOTAL_BIGRAMS;

        upperlower /= TOTAL_BIGRAMS;

        hands.a /= TOTAL_CHARACTERS;
        hands.b /= TOTAL_CHARACTERS;

        for(int i = 0; i < 8; i++)
            fingers[i] /= TOTAL_CHARACTERS;
        
        capspinky /= TOTAL_CAPITALS;
    }

public:
    string layout;

    double double_inward;
    double triple_inward;
    double quad_inward;

    double double_same;
    double triple_same;
    double quad_same;

    double alternation;

    double upperlower;

    Pair<double, double> hands;

    vector<double> fingers;

    double capspinky;

    Keyboard(string s): layout(s), hands(0,0), fingers(8, 0) {
        double_inward = 0;
        triple_inward = 0;
        quad_inward = 0;

        double_same = 0;
        triple_same = 0;
        quad_same = 0;

        alternation = 0;

        upperlower = 0;

        capspinky = 0;
    }
    //

    void quick_print() {
        print_buffer("", 25); print_buffer(layout.substr(0, 10), 15); cout << endl;
        print_buffer("", 25); print_buffer(layout.substr(10, 10), 15); cout << endl;
        print_buffer("", 25); print_buffer(layout.substr(20, 10), 15); cout << endl;

        print_line("Double inward-rolls",  double_inward);
        print_line("Double same-finger", 1 - double_same);
        print_line("Percent alternation", alternation);
        print_line("Percent upper-lower", 1 - upperlower);

        cout << endl;
        print_line("Left-hand", hands.a);
        print_line("Right-hand", hands.b);

        cout << endl;
        print_line("Left-Pinky", fingers[0]);
        print_line("Left-Ring", fingers[1]);
        print_line("Left-Middle", fingers[2]);
        print_line("Left-Pointer", fingers[3]);
        print_line("Right-Pointer", fingers[4]);
        print_line("Right-Middle", fingers[5]);
        print_line("Right-Ring", fingers[6]);
        print_line("Right-Pinky", fingers[7]);

        cout << endl;
        print_line("Caps-Pinky", capspinky);
    }

    // void get_stats(const Keyboard& k) {
    //     // Bigrams/ trigrams/ quadgrams
    //     for(std::tr1::unordered_map<string, int>::const_iterator it = QUADS.begin(); it != QUADS.end(); ++it)
    //         add_stat(it->first, it->second);
        
    //     // Hand usage
    //     for(int i = 0; i < 30; i++) {
    //         if (i % 10 < 5) { // Left
    //             hands.a += singles[get_index(k.layout[i])];
    //         }
    //         else { // Right
    //             hands.b += singles[get_index(k.layout[i])];
    //         }
    //     }

    //     // Finger usage
    //     for(int i = 0; i < 30; i++) {
    //         int column = i % 10;
    //         if (column <= 3)
    //             fingers[column] += singles[get_index(k.layout[i])];
    //         else if (column <= 5)
    //             fingers[column - 1] += singles[get_index(k.layout[i])];
    //         else
    //             fingers[column - 2] += singles[get_index(k.layout[i])];
    //     }

    //     div_total();
    // }
};

#endif