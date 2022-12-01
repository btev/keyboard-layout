#ifndef __GENERAL_INPUT__
#define __GENERAL_INPUT__

#include<iostream>
#include<vector>
#include"general.h"
#include <conio.h> // getch for instant character input
using namespace std;

int input_integer(const string& question, int buffer = 0) {
    int rtn;
    if (buffer) {
        print_buffer(question, buffer);
        cout << ": ";
    }
    else
        cout << question;
    cin >> rtn;
    return rtn;
}
double input_double(const string& question, int buffer = 0) {
    double rtn;
    if (buffer) {
        print_buffer(question, buffer);
        cout << ": ";
    }
    else
        cout << question;
    cin >> rtn;
    return rtn;
}

bool input_yn(const string& question, int buffer = 0) {
    char rtn;
    if (buffer) {
        print_buffer(question, buffer);
        cout << ": ";
    }
    else
        cout << question;
    rtn = getch();
    if (rtn == 'y' || rtn == 'Y')
        return true;
    return false;
}

bool input_yn(int buffer = 0) {
    char rtn;
    if (buffer) {
        print_buffer("Yes or no (y/n)?", buffer);
        cout << ": ";
    }
    else
        cout << "Yes or no (y/n)?: ";
    rtn = getch();
    if (rtn == 'y' || rtn == 'Y')
        return true;
    return false;
}





#endif