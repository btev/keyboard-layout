#ifndef __KEYBOARD_LOCKS__
#define __KEYBOARD_LOCKS__

#include<iostream>
#include<vector>
#include<bits/stdc++.h> // vector sort
using namespace std;

#include"../general/general.h"

// Lock super-class of v32
class Lock: public v32 {
public: 
    int letter;
    Lock(int l): letter(l) {}
    Lock(int l, const vector<int>& v): v32(v), letter(l) {}
};

class Locks {
    // Filling empty chars with 0-31, updating keys accordingly
    void fill_empty() {
        for(int i = 0; i < 30; i++) {
            if (!chars[i].empty())
                continue;

            // filling char
            for(int j = 0; j < 30; j++)
                chars[i].push_back(j);
            
            // adding onto keys
            for(int j = 0; j < 30; j++)
                keys[j].push_back(i);
        }
    }
    // Filling empty chars with remaining keys for each hand
    void set_hands(int left_bits = 1066484625) {
        // calculating left and right hands
        vector<int> left_keys;
        vector<int> right_keys;
        for(int i = 0; i < 30; i++) {
            if (!keys[i].empty())
                continue;

            if (i % 10 < 5)
                left_keys.push_back(i);
            else
                right_keys.push_back(i);
        }

        // adding left and right to chars
        int bit = 1;
        for(int i = 0; i < 30; i++) {
            if (!chars[i].empty())
                continue;
            
            if (left_bits & bit) {
                set_letter(i, left_keys);
            }
            else {
                set_letter(i, right_keys);
            }
            bit <<= 1;
        }
    }
    // Updating array of sorted chars
    void update_sort() {
        vector<Pair<int, int>> tmp(30); // a: index, b: size
        for(int i = 0; i < 30; i++) {
            tmp[i].a = i;
            tmp[i].b = chars[i].size();
        }
        sort(tmp.begin(), tmp.end(), pair_compare_b_reverse<int, int>);

        for(int i = 0; i < 30; i++)
            c_asc[i] = tmp[i].a;
    }
    // don't run this if chars[l.letter] isn't empty
    void set_letter(const Lock& l) {
        chars[l.letter] = l;
        for(int i: l)
            keys[i].push_back(l.letter);
    }
    void set_letter(int c, const v32& v) {
        chars[c] = v;
        for(int i: v)
            keys[i].push_back(c);
    }

    // for updating locks without clearing everything
    void clear_key(int key) {
        // Looping through each character from said key
        for(int c: keys[key]) {
            // Remove said key from that characters key list
            chars[c] = copy_without(chars[c], key);
        }
        keys[key].clear();
    }

    void clear_char(int c) {
        // Looping through each key from said character
        for(int k: chars[c]) {
            // Remove said character from that keys character list
            keys[k] = copy_without(keys[k], c);
        }
        chars[c].clear();
    }
public: 
    vector<v32> chars;
    vector<v32> keys;
    vector<int> c_asc; // chars ascending

    Locks(): chars(30), keys(30), c_asc(30) {}


    void change_locks(const vector<Lock>& v) {
        chars.clear();
        keys.clear();

        // Setting the letters
        for(Lock l: v)
            set_letter(l);
        
        fill_empty();
        update_sort();
    }

    void change_locks_hands(const vector<Lock>& v, int left_bits = 1066484625) {
        chars.clear();
        keys.clear();

        // Setting the letters
        for(Lock l: v)
            set_letter(l);
        
        set_hands(left_bits);
        update_sort();
    }

    // pretty slow
    void lock_char(int character, int key) {
        clear_char(character);
        clear_key(key);
        chars[character].push_back(key);
        keys[key].push_back(character);
    }

    //
    v32& get_sorted(int i) {
        return chars[c_asc[i]];
    }

    // print
    void print() {
        cout << endl << "Char-to-keys:" << endl;
        for(int i = 0; i < 30; i++) {
            cout << string(5, ' ') << get_char(i) << ": ";
            for(int k: chars[i])
                cout << k << " ";
            cout << endl;
        }

        cout << endl << "Key-to-chars:" << endl;
        for(int i = 0; i < 30; i++) {
            cout << string(5, ' ') << i << ": ";
            for(int c: keys[i])
                cout << get_char(c) << " ";
            cout << endl;
        }
    }
};

#endif