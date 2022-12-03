#ifndef __STRING_CHAR__
#define __STRING_CHAR__

#include<iostream>
#include<vector>
using namespace std;

// * * String
string swap(string s, int a, int b) {
    char tmp = s[a];
    s[a] = s[b];
    s[b] = tmp;
    return s;
}

void print_buffer(string s, unsigned int b) {
    if ( s.size() >= b) {
        cout << s.substr(0, b);
        return;
    }
    cout << string(b - s.size(), ' ') << s;
}

void print_line(string s, unsigned int b, double data) {
    print_buffer(s, b);
    cout << ": " << to_string(data) << endl;
}


// * * Char
bool isAlphabetical(char& c) { // Also changes 'A' -> 'a'
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}







#endif