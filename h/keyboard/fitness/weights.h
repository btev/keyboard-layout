#ifndef __KEYBOARD_CALC_2__
#define __KEYBOARD_CALC_2__

#include<iostream>
#include<vector>
using namespace std;
#include"../../general/input.h"

/*
Maybe should have all the weights in one big array.
Then I could have a bunch of strings with the names in a large array aswell, allowing for easy printing
*/

class Weights {
public:
    // General key weights just based on their position on the keyboard
    vector<double> KEY_WEIGHTS{
        1.1, 1.07, 1.05, 1.05, 1.07,  1.2, 1.05, 1.05, 1.07,  1.1,
        1.05, 1.02,    1,    1,  1.1,  1.1,    1,    1, 1.02, 1.05,
        1.1,  1.1, 1.05, 1.05,  1.2,  1.1, 1.05, 1.05,  1.1,  1.1
    };

    double CAPS_BASE = 1.5;

    // General key weights for having to hold shift and hit a letter in the pinky column
    vector<double> CAPS_PINKY{
        1.7,
        1.6,
        1.75
    };

    // vector<double> SAME_FINGER_BIGRAM{
    //     1,    // Same-key 1.4
    //     4.4,    // Home-Lower
    //     6,      // Upper-Lower
    //     4.4,     // Home-Upper
    //     5.2     // Pointer to middle column
    // };
    vector<double> SAME_FINGER_BIGRAM{
        1,    // Same-key 1.4
        2.2,    // Home-Lower
        3,      // Upper-Lower
        2.2,     // Home-Upper
        2.6     // Pointer to middle column
    };

    vector<double> SAME_FINGER_TRIGRAM{
        1,
        1.4,
        2.2,
        1.4,
        1.8
    };
    vector<double> SAME_FINGER_QUADGRAM{
        1,
        1,
        1.4,
        1,
    };

    // After typing a capital letter that was on the same column as the left-pinky, you have to type a key in the ring finger column.
    // Not using this one currently because capitals slow down everything too much
    vector<double> CAPS_PINKY_BIGRAM{
        1.1,
        1.15,
        1.8
    };
    vector<double> CAPS_PINKY_TRIGRAM{
        1,
        1,
        1.41
    };
    vector<double> CAPS_PINKY_QUADGRAM{
        1,
        1,
        1.1
    };

    // Having to type a key from a different row on the same hand
    vector<double> ROW_JUMP{
        1,    // Same-Row
        1.1,   // Home-Lower
        1,      // placeholder
        1.05   // Home-Upper
    };

    vector<double> DOUBLE_ROW_JUMP{
        1.05,      // Low-Pointer
        1.25,   // High-Pointer -> Pinky
        2,      // High-Pointer

        1.1,      // High-Middle
        1.5,    // Low-Middle -> Pinky
        2,      // Low-Middle

        1.25,   // Low-Pinky
        2       // Low-Ring
    };

    // General triple scenarios and the weights associated
    vector<double> TRIGRAM_WEIGHTS{
        2,      // Inside-Redirect: The first two keys roll with a gap in-between, where the final letter is located. Ex: "das", "ljk"
        1.5,    // Pinky-Outward:   An outward roll that contains the pinky
        1.3,    // Center-Outward:  An outward roll with the pointer on the center column
        1.2,    // Redirect:        Rolling in a direction and then having to swap direction (same-hand)
        0.8,    // Roll-Alternate:  The first two keys roll and the last one alternates
        0.8,    // Full-Outward:    An outward roll that doesn't belong to one of the categories mentioned above
        0.65    // Full-Inward:     An inward roll
    };

    vector<double> QUADGRAM_WEIGHTS{
        0.8,    // Full Outward: A four character outward-roll
        0.85,   // Roll-Alternate: A triple roll and the final key swaps hands
        0.8     // Full-Inward: A full four character inward roll (note this is on top of the triple 0.65 multiplier, so really it's 0.52x)
    };

    // Overall Bonuses (Note these are just the desired percentages in each category, for every category the layout hits it's entire fitness it multiplied by USAGE_MULIPLIER, 0.99x )
    vector<double> FINGER_USAGE{
        0.08,
        0.12,
        0.16,
        0.14,
        0.14,
        0.16,
        0.12,
        0.08,
    };

    // * * Functions

    void cin_keyboard() {
        if (input_yn("Completely change all the weights?: ")) {
            for(int i = 0; i < 30; i++) {
                KEY_WEIGHTS[i] = input_double("Set the weight of position" + to_string(i) + " to? (Currently " + to_string(KEY_WEIGHTS[i]) + "): ");
            }
        }
        else if (input_yn("Change a few key weights?: ")){
            
            while(true) {
                int key = input_integer("What key weight would you like to change?(Enter -1 to stop): ");
                if (key == -1)
                    break;
                KEY_WEIGHTS[key] = input_double("Set the weight of position" + to_string(key) + " to? (Currently " + to_string(KEY_WEIGHTS[key]) + "): ");
            }
        }

        if (input_yn("Change the base capital key weight?: ")) {
            CAPS_BASE = input_double("Set the weight the caps key to? (Currently " + to_string(CAPS_BASE) + "): ");
        }

        if(input_yn("Change the weight of the caps-pinky combo?: ")) {
            for(int i = 0; i < 3; i++) {
                CAPS_PINKY[i] = input_double("Set the weight of caps-pinky position" + to_string(i) + " to? (Currently " + to_string(CAPS_PINKY[i]) + "): ");
            }
        }
    }


    void cin_bigrams() {

    }

    void cin_trigrams() {

    }

    void cin_quadgrams() {

    }

    void cin_weights() {
        if (!input_yn("Would you like to modify the algorithm weights?: "))
            return;
        
        if (input_yn("Keyboard key weights?: "))
            cin_keyboard();

        if (input_yn("Bigram weights?: ")) {
            cin_bigrams();
        }

        if (input_yn("Trigram weights: ")) {
            cin_trigrams();
        }

        if (input_yn("Quadgram weights?: ")) {
            cin_quadgrams();
        }
    }
};


#endif