#ifndef __KEYBOARD_CALC__
#define __KEYBOARD_CALC__

#include<iostream>
#include<vector>
using namespace std;

#include"read_in.h"


int SAME_FINGER;
int SAME_FINGER_TRIGRAM;
int SAME_FINGER_QUADGRAM;
int SAME_HAND; // not sure if this should be a de-buff, as many other modifiers break this down into its parts
int SAME_HAND_TRIGRAM; // good if in-ward
int SAME_HAND_QUADGRAM;

int FINGER_DISTANCE;

int SAME_ROW;
int UPPER_HOME;
int LOWER_HOME;
int UPPER_LOWER;

int INWARD_ROLL;
int INWARD_ROLL_TRIGRAM;
int INWARD_ROLL_QUADGRAM;

int TRIGRAM_REVERSE;


/*
* Multipliers
    * * Singles
        * * * Key Weights / Travel Distance
            I'm a bit different from a lot of the popular layouts today, I don't think that minimum-travel-distance and the the keys position need to be weighed so heavily.
            To me avoiding the lower->upper row combos, same-finger-bigrams and redirects are much more important. When I pay attention to myself typing on Qwerty, the time-loss and
                discomfort to hit keys like 'q' or 'p' is barely there. Especially relative to same-finger-bigrams and the other metrics I mentioned earlier. Of course I would rather
                hit a key on the home-row with my pointer finger over some pinky-finger bottom row key, but if it increases the same-finger-bigram rate significantly, I'll choose the pinky.
            Anyways that's the reason I barely debuff the corner pinky spots and other hard to reach locations. 

            1.1, 1.07, 1.05, 1.05, 1.07,  1.2, 1.05, 1.05, 1.07,  1.1,
            1.05, 1.02,   1,    1,  1.1,  1.1,    1,    1, 1.02, 1.05,
            1.1,  1.1, 1.07, 1.05,  1.3,  1.1, 1.05, 1.07,  1.1,  1.1

            This isn't really a proper travel distance measurement, but I think it's fine for now. Maybe I'll implement a proper one in the future
        * * * Capitalization
            * * General
                Well it is hitting two-keys, but the capilization is very easy to recognize and the shift key is set, so I wouldn't value it as high as a normal two-key combo like "fj" (valued at 2.0)
                Base : 1.5x

            * * Left-Pinky
                I use the left-pinky for shift so this debuff is based on that. zebra quesiton kddkdkA
                I actually don't think this is that bad, not much worse than base and definitely not close to bigrams like "yn" yet... future combos like "Aw" will be though.

                Upper : 1.7x
                Home  : 1.6x
                Lower : 1.75x

    * * Bigrams
        - Evalutate both characters and add them up
        - Add any modifiers from the two character perspective
        * * * Same Finger
            If true, don't bother checking the other buffs

            Same-Key   : 1.4x
            Up-Or-Down : 2.2x
            Top-Bottom : 3x

                * * Capitals
                    Things like "Aw" that aren't normally counted as same-finger-bigrams should be added here.
                    That being said, I don't think it's as bad as a normal SFB.
                    Might re-work this to also consider same-hand maneuvers after typing a capital with pinky, definitely can mess up your hand-position.

                    Same-Row   : 1.1x  (1.3x -> 1.43x)    // A Normal SFB gets this low of a debuff because it's just hitting the same key-twice, no movement. This gets a low value because generally my index finger would already be in the correct position for key two.
                    Up-Down    : 1.15x (1.36x -> 1.58x)    // I find this to not be too terrible for similar reasons to the first one, it's just a little more awkward.
                    Top-Bottom : 1.8x  (1.41x -> 2.53x)    // This one is pretty close to normal Top-Bottom bigrams
        
        * * * Single row jump (same hand)
            I initially thought the default key position weights aleady did this, but I've decided these should be added

            Home-Upper : 1.1x  (1.07x -> 1.17x)
            Upper-Home : 1.05x (1.07x -> 1.12x)
            Home       : 1.0x  (1x -> 1.0x)
            Lower-Home : 1.05x (1.1x -> 1.15x)
            Home-Lower : 1.1x  (1.1x -> 1.21x)
        
        * * * Double row jump (same hand)
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

        * * * Rolls
            At this stage it's really just alternation vs. same-hand,  everything on the same hand is already a roll
            It's all summed up pretty well in the single/double row jumps, no need to add any buff/debuffs

    * * Trigrams
        - Evaluate both bigrams and add them together
        - Add any modifiers from the three character perspective
        - Multiply by 3/4 as we had added up four characters worth of value instead of three

        * * * Same-Finger (The beginning and end of Trigram)
            Bigram values shifted

            Same-Key   : 1x
            Up-Or-Down : 1.4x
            Top-Bottom : 2.2x

            * * Capitals
                Things like "Aus"
                I don't know why but this one almost feels worse than the bigram version,
                like with the bigram version my brain is ready to hit the next key even though my hand it out of position.
                With this, my brain switches back to normal, but before I can re-center myself I'm thrown back into this weird hand position game
                Usually I end up using the pinky for both ends of the trigram, but I'm going to assume you could learn to use the other finger instead

                Same-Row   : 1x  (1.3x)
                Up-Down    : 1x  (1.36x)  
                Top-Bottom : 1.41x  (1.41x -> 2x)
        
        * * * General
            Does consider inward rolls with "trash" combos in them, but they will be severely debuffed from the bigram calculation anyways
            Feels like Up-Or-Down same-finger is a similar debuff to a redirect.
            I keep going back and forth on outward-rolls, very difficult to pin down. Sometimes I feel like I can do them very well, other times it messes with my brain
            * Re-Evaluate Outward-Rollss

            Inside-Redirect  : 2x    (1x to 2x)     // Feels super awkward, messes with my brain
            Pinky-Outward    : 1.5x  (1x to 1.5x)   // Outward roll that used the pinky
            Middle-Outward   : 1.3x  (1x to 1.3x)   // Outward roll with the pointer finger on the middle column
            Redirect         : 1.2x  (1x to 1.2x)
            Roll-Alternate   : 0.8x  (1x to 0.8x)
            Full-Outward     : 0.8x  (1x to 0.8x)   // With neither of the mentioned above
            Full-Inward      : 0.65x (1x to 0.65x)
            // Full-Alternating : 1.0x the two bigrams should calulate this correctly

    * * Quadgrams
            - Evaluate both trigrams and add them together
            - Add any modifiers from the four character perspective
            - Multiply by 4/6 as we added up six characters instead of four

            * * * Same-Finger (The beginning and end of Trigram)
            Trigam values shifted

            Same-Key   : 1x
            Up-Or-Down : 1x
            Top-Bottom : 1.4x

            * * Capitals
                Things like "Ques"
                Still not able to re-center hands unless it's same-row. But overall not that bad.

                Same-Key   : 1x
                Up-Or-Down : 1x
                Top-Bottom : 1.1x (1.41x -> 1.551x)
        
        * * * General
            // Inside-Redirect  : 1.5x should already be calculated from trigrams
            // Redirect         : 1.2x should already be calculated from trigrams
            Full-Alternating : 1.0x
            Full-Outward     : 0.8x  (0.8x -> 0.64x)
            Roll-Alternate   : 0.85x  (0.725x -> 0.616x inward, 0.8x -> 0.68x outward) 
            Full-Inward      : 0.8x   (0.65x -> 0.52x)
    * * Overall
        * * * Finger Usage / Hand Usage
            I think this is represented fairly well with the key weights, but it might be important to just list some debuffs.
            The values are really small cause I'll be multiplying the entire fitness
            All values within 1%

            Pinky 8%     : 0.99x
            Ring  11%    : 0.99x
            Middle 16%   : 0.99x
            Pointer 15%  : 0.99x

            Hand 50% each: 0.99x

            Figured home-row usage being higher than the other rows was probably guarenteed, so I didn't include that

    * Process
        I've been reading a ton, taking in everyone's keyboard ideals and opinions from layouts like
        
        Colemak, Halmak, Dvorak, Workman, Norman, Kaehi, Ornate, Asset, Capewell, ect ect

        The layout I've found to agree most with so far is the MTGAP layouts, been reading some old
            blogish/discussion posts that have been talking a lot about the ideas I've been thinking about myself.
            Here's a link to some cool info https://mathematicalmulticore.wordpress.com/2011/11/27/typing-data-preliminary-analysis/#comments
        
        A lot of the other layouts seem to highly focus on key-positions and distance traveled. Which is nice because it's a clean and easy to calculate statistic, but I just
            don't think it's *that* important.
          
          
        Probably the biggest thing though has just been me typing away in this document, trying different finger/key maneuvers and comparing there speed/comfort to others. Hopefully
            through time these weights will become more and more refined.

    * Working out Ideas

    
    Not sure exactly how to value a bigram vs a trigram vs a quadgram.
    For an average trigram I'd like to add up all its bigram first, but that would mean that trigrams would be very heavily weighted,
        is that ok because they are less common? Or instead should I push all gram base values to be around 1.
    For example a bigram like "dj" is just a normal alternating bigram so I could value it at 1, the same goes for bigram "jf". But then I have the trigram "djf",
        should the base value of "djf" be 2, or 1? If it's 2 that might result in the fitness of a keyboard being mainly based on trigrams.
    I guess there are three options for compiling the smaller bigrams, either add them, multiply them, or average them.

    Ran into an issue

    "word"
    2 characters in singles
    2 characters in bigrams
    __
    4 total
    / 4 characters
    __
    1 value per character

    "generic"
    7 characters in singles
    12 characters in doubles
    15 characters in triples
    16 characters in quads
    ___
    47 total
    / 7
    __
    7ish value per character

    Obviously this isn't the way to go, but I still want to consider things like quad rolls, as they would speed up typing a ton.
    I'm thinking to save everything as a quad, including spaces, and just calculate every quad as a layouts fitness
    "east" would be calculated like a normal quad
    "er e" would be calculate as a the double "er" + single "e"
    "gg. " would be calculated as a the triple "gg."

    I think this works because every character will be evaluated 4 times. This solves the problem of how much a quad should be worth, it should be two doubles or four singles worth of fitness.
*/



// Weight Calculations
double fitness(vector<int> v) {
    // Bigrams
    double rt =  0;
    for(std::tr1::unordered_map<string, int>::const_iterator it = doubles.begin(); it != doubles.end(); ++it){
        rt += bi_weights[v[get_index(it->first[0])]][v[get_index(it->first[1])]] * it->second;
    }

    return rt;
}

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

void anecdotal_weights() {
    // Weights
    for(int i = 0; i < 30; i++) {
        for(int j = 0; j < 30; j++) {
            bi_weights[i][j] = get_weight(i, j);
        }
    }
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

    void get_stats(const Keyboard& k) {
        // Bigrams/ trigrams/ quadgrams
        for(std::tr1::unordered_map<string, int>::const_iterator it = quads.begin(); it != quads.end(); ++it)
            add_stat(it->first, it->second);
        
        // Hand usage
        for(int i = 0; i < 30; i++) {
            if (i % 10 < 5) { // Left
                hands.a += singles[get_index(k.layout[i])];
            }
            else { // Right
                hands.b += singles[get_index(k.layout[i])];
            }
        }

        // Finger usage
        for(int i = 0; i < 30; i++) {
            int column = i % 10;
            if (column <= 3)
                fingers[column] += singles[get_index(k.layout[i])];
            else if (column <= 5)
                fingers[column - 1] += singles[get_index(k.layout[i])];
            else
                fingers[column - 2] += singles[get_index(k.layout[i])];
        }

        div_total();
    }
};

#endif