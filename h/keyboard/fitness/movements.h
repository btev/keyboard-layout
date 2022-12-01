#ifndef __FITNESS_MOVEMENTS__
#define __FITNESS_MOVEMENTS__

#include<iostream>
#include<vector>
using namespace std;


/*
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
*/

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
double top_bottom(const vector<Letter>& v) {
    int a = v[0].get_finger();
    int b = v[1].get_finger();

    if (a == 1) { // If pointer
        if (v[0].row == 3) // Lower
            return ROW_JUMP[1];  // Not that bad, multiply as if it was a home-lower combo

        if (b == 4) // If pinky
            return 1.25;         // Definitely annoying

    }
    else if (a == 2) {// If Middle
        if (v[0].row == 1) // Upper
            return ROW_JUMP[1];  // Not that bad, multiply as if it was a home-lower combo

        if(b == 4) // If pinky
            return 1.5;          // Pretty awkward hand spot
    }
    else if (b == 1) { // If pointer
        if (v[1].row == 3) // Lower
            return ROW_JUMP[1];  // Not that bad, multiply as if it was a home-lower combo

        if (a == 4) // If pinky
            return 1.25;         // Definitely annoying

    }
    else if (b == 2) {// If Middle
        if (v[1].row == 1) // Upper
            return ROW_JUMP[1];  // Not that bad, multiply as if it was a home-lower combo

        if(a == 4) // If pinky
            return 1.5;          // Pretty awkward hand spot
    } // The remaining two fingers must be pinky and ring
    else if (b == 3) { // Ring
        if (v[1].row == 1)
            return 1.25;         // The last scenario I'm checking for, Lower-Pinky -> Upper-Ring
    }
    else { // Pinky
        if (v[1].row == 3)
            return 1.25;
    }
    return 2.0;
}

double calculate_bigram(const vector<Letter>& v) {
    double rtn = calculate_key(v[0]) + calculate_key(v[1]); // Adding up base letter values 

    // Alternate-Hand
    if ( v[0].hand != v[1].hand)
        return rtn;
    
    // Same-finger
    if ( (v[0].finger) == (v[1].finger) )
        return rtn * SAME_FINGER_BIGRAM[v[0].row ^ v[1].row];

    if ( !(v[0].row + v[1].row == 4) )
        return rtn * ROW_JUMP[v[0].row ^ v[1].row];

    if (!v[0].hand)
        return rtn * top_bottom(v);    // Left hand

    return rtn * top_bottom(v);       // Right hand
}

double calculate_trigram(const vector<Letter>& v) {
    double rtn = ( calculate_bigram(remove_index(v, 0))  + calculate_bigram(remove_index(v, 2)) ) * (3.0 / 4.0);

    if ( v[0].hand != v[2].hand ) {
        if (v[0].hand == v[1].hand)
            return rtn * TRIGRAM_WEIGHTS[4];        // Roll-Alternate
        return rtn;
    }

    if (v[0].finger == v[2].finger)
        return rtn *= SAME_FINGER_TRIGRAM[v[0].row ^ v[2].row];

    int finger_1 = v[0].get_finger();
    int finger_2 = v[1].get_finger();
    bool inward = (finger_1 - finger_2) > 0;

    switch(finger_1 ^ finger_2) {
        case 0:         // same-finger
            return rtn;
        case 3:         // 1 ^ 2, Pointer + Middle
            if (inward)
                return rtn *= TRIGRAM_WEIGHTS[3];       // Redirect

            if (v[2].get_finger() == 4) // pinky
                return rtn *= TRIGRAM_WEIGHTS[1];       // Pinky-Outward
            
            if (v[0].column == 5)                       
                return rtn *= TRIGRAM_WEIGHTS[2];       // Center-Outward
        
            return rtn *= TRIGRAM_WEIGHTS[5];           // Full-Outward
        case 2:         // 1 ^ 3, Pointer + Ring
            if (v[2].get_finger() == 2)
                return rtn *= TRIGRAM_WEIGHTS[0];       // Inside-Redirect
            if (inward)
                return rtn *= TRIGRAM_WEIGHTS[3];       // Redirect

            if (v[2].get_finger() == 4) // pinky
                return rtn *= TRIGRAM_WEIGHTS[1];       // Pinky-Outward
            
            if (v[0].column == 5)                       
                return rtn *= TRIGRAM_WEIGHTS[2];       // Center-Outward
        
            return rtn *= TRIGRAM_WEIGHTS[5];           // Full-Outward
        case 5:         // 1 ^ 4, Pointer + Pinky
            return rtn *= TRIGRAM_WEIGHTS[0];           // Inside-Redirect
        case 1:         // 2 ^ 3, Middle + Ring
            if (inward) {
                if (v[2].get_finger() == 1)
                    return rtn *= TRIGRAM_WEIGHTS[6];   // Full-Inward
                return rtn *= TRIGRAM_WEIGHTS[3];       // Redirect
            }
            if (v[2].get_finger() == 4)
                return rtn *= TRIGRAM_WEIGHTS[1];       // Pinky-Outward
            return rtn *= TRIGRAM_WEIGHTS[5];           // Full-Outward
        case 6:         // 2 ^ 4, Middle, Pinky
            if (v[2].get_finger() == 3)
                return rtn *= TRIGRAM_WEIGHTS[0];       // Inside-Redirect
            if (inward)
                return rtn *= TRIGRAM_WEIGHTS[6];       // Full-Inward
            return rtn *= TRIGRAM_WEIGHTS[3];           // Redirect
        default: // (7)    3 ^ 4, Ring, Pinky
            if (inward)
                rtn *= TRIGRAM_WEIGHTS[6];              // Full-Inward
            return rtn *= TRIGRAM_WEIGHTS[3];           // Redirect
    }
}
double calculate_quadgram(const vector<Letter>& v) {
    double rtn = ( calculate_trigram(remove_index(v, 0))  + calculate_trigram(remove_index(v, 3)) ) * (4.0 / 6.0);

    if ( v[0].hand != v[2].hand ) {
        if ((v[0].hand == v[1].hand) && (v[1].hand == v[2].hand))
            return rtn * QUADGRAM_WEIGHTS[1];            // Roll-Alternate
        return rtn;
    }

    if (v[0].finger == v[3].finger)
        return rtn *= SAME_FINGER_QUADGRAM[v[0].row ^ v[2].row];

    int finger_1 = v[0].get_finger();
    int finger_2 = v[1].get_finger();
    int finger_3 = v[2].get_finger();

    if ( (finger_2 ^ finger_3) == 1) { // Middle + Ring in the middle is needed for a roll
        if (finger_2 == 2) { // Potential outward roll
            if (finger_1 == 1 && v[3].get_finger() == 4)
                return rtn *= QUADGRAM_WEIGHTS[0];          // Full-Outward
        }
        else {               // Potential inward roll
            if (finger_2 == 4 && v[3].get_finger() == 1)
                return rtn *= QUADGRAM_WEIGHTS[2];          // Full-Inward
        }

    }

    return rtn;
}

// 923521 combos btw
double get_weight(const vector<Letter>& quad) {
    double rtn = 0;

    vector<Letter> v;
    for(int i = 0; i < 4; i++) {
        if (quad[i].position == 30) {
            switch(v.size()) {
                case 1:
                    rtn += calculate_key(v.back().character);
                    break;
                case 2:
                    rtn += calculate_bigram(v);
                    break;
                case 3:
                    return rtn + calculate_trigram(v);
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
            return rtn + calculate_bigram(v);
            break;
        case 3:
            return calculate_trigram(v);
        case 4:
            return calculate_quadgram(v);
        default: // (0)
            return rtn;
    }
}

void anecdotal_weights() {
    // Weights
    for(int i = 0; i < 31; i++) {
        for(int j = 0; j < 31; j++) {
            for(int x = 0; x < 31; x++) {
                for(int y = 0; y < 31; y++) {
                    ALL_QUADGRAM_WEIGHTS[i][j][x][y] = calculate_quadgram({i, j, x, y});
                    // if (ALL_QUADGRAM_WEIGHTS[i][j][x][y] > 15) {
                    //     cout << i << " " << j << " " <<  x << " " << y << endl;
                    // }
                }
            }
        }
    }
}
*/

#endif