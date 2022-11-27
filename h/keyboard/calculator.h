#ifndef __KEYBOARD_CALC__
#define __KEYBOARD_CALC__

#include<iostream>
#include<vector>
using namespace std;

#include"read_in.h"

// Weight Calculations
double fitness(vector<int> v) {
    // Bigrams
    double rt =  0;
    for(std::tr1::unordered_map<string, int>::const_iterator it = doubles.begin(); it != doubles.end(); ++it){
        rt += bi_weights[v[get_index(it->first[0])]][v[get_index(it->first[1])]] * it->second;
    }

    return rt;
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