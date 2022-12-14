#ifndef __KEYBOARD_GENERAL__
#define __KEYBOARD_GENERAL__

#include<iostream>
#include<vector>
using namespace std;

// * * Functionss
// Turns the randomized keyboard characters to a number between 0->29
int get_index(char c) {
    switch(c) {
        case ';':
            return 26;
        case ',':
            return 27;
        case '.':
            return 28;
        case '/':
            return 29;
        default:
            if (c < 'a')
                return c - 'A';
            return c - 'a';
    }
}
char get_char(int i) {
    switch(i) {
        case 26:
            return ';';
        case 27:
            return ',';
        case 28:
            return '.';
        case 29:
            return '/';
        default:
            if (i < 0)
                return i + 'A';
            return i + 'a';
    }
}

bool onKeyboard(char c) {
    switch (c) {
        case ';':
            return true;
        case ',':
            return true;
        case '.':
            return true;
        case '/':
            return true;
        default:
            if ('a' <= c && c <= 'z')
                return true;
            return ('A' <= c) && (c <= 'Z');
    }
}

// string to vec
string vector_to_string(const vector<int> v) {
    string rtn(30, ' ');
    for(int i = 0; i < 30; i++) {
        rtn[v[i]] = get_char(i);
    }
    return rtn;
}
vector<int> string_to_vector(string s) {
    vector<int> rtn(30);
    for(int i = 0; i < 30; i++) {
        rtn[get_index(s[i])] = i;
    }
    return rtn;
}

// Prints string in a keyboard like fashion
void print_keyboard(string s, int spacing = 0) {
    for(int i = 0; i < 3; i++) {
        cout << string(spacing, ' ');
        for(int j = 0; j < 10; j++) {
            cout << s[(i * 10) + j] << " ";
        }
        cout << endl;
    }
}
void print_keyboard(const vector<int>& v, int spacing = 0) {
    string s = vector_to_string(v);
    for(int i = 0; i < 3; i++) {
        cout << string(spacing, ' ');
        for(int j = 0; j < 10; j++) {
            cout << s[(i * 10) + j] << " ";
        }
        cout << endl;
    }
}


// Classes
class Key {
public:
    bool hand;
    int row;
    int finger;
    Key() {}
    Key(int p) {
        setKey(p);
    } 

    void setKey(int position) {
        int column = position % 10;
        row = position / 10;

        // Finger
        if (column <= 3)
            finger = column;
        else if (column <= 5)
            finger = column - 1;
        else
            finger = column - 2;

        // Hand
        if (column <= 4)
            hand = 0;
        else
            hand = 1;
    }
};

#endif