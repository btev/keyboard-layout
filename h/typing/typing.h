#ifndef __TERMINAL_TYPING__
#define __TERMINAL_TYPING__

#include<iostream>
#include<fstream>
#include<chrono>
#include"../keyboard/general.h"
#include"../general/general.h"
#include <conio.h> // getch for instant character input
#include<windows.h>
using namespace std;


// * * Chrono
int duration(const auto& a, const auto& b) {
    return chrono::duration_cast<chrono::microseconds>(b - a);
}

int since(const auto& a) {
    auto now = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::microseconds>(now - a).count();
}

// * * Random word/line
string random_word(uint size) {
    string rtn;

    while (rtn.size() < size)
        rtn.push_back(get_char(rand() % 30));

    return rtn;
}
string random_word(int min, int max) {
    if (min == max)
        return random_word(min);
    string rtn;

    uint size = (rand() % (max - min)) + min + 1;
    while (rtn.size() < size)
        rtn.push_back(get_char(rand() % 30));

    return rtn;
}
// Random line
string random_line(int length, int min = 3, int max = 6) {
    string rtn;

    while(rtn.size() < uint(length) ) {
        rtn.append(random_word(min, max));
        rtn.push_back(' ');
    }
    rtn.pop_back();
    return rtn;
}

string random_line_exact(int length, int min = 3, int max = 6) {
    string rtn;

    vector<int> word_lengths = add_to_random(vector_from_to(min + 1, max + 1), length); // +1 because of spaces

    for(int word: word_lengths) {
        rtn.append(random_word(word - 1));
        rtn.push_back(' ');
    }

    rtn.pop_back();
    return rtn;
}



string paragraph(vector<string> v) {
    string rtn;
    for(string s: v)
        rtn.append(s);
    return rtn;
}
// * * Load and Save weights
// void write_weights() {
//     ofstream file("weights.txt");

//     for(int i = 0; i < 30; i++) {
//         for(int j = 0; j < 30; j++) {
//             file << bi_data[i][j].speed << "-" << bi_data[i][j].o << " ";
//         }
//         file << endl;
//     }
// }

// void read_weights() {
//     ifstream file("weights.txt");
//     if (file.fail())
//         return;

//     char c;
//     int row = 0;
//     int column = 0;
//     string value;
//     while (file.get(c)) {
//         if (c == '-') {
//             bi_data[row][column].speed = stoi(value);
//             value.clear();
//             continue;
//         }
//         if (c != ' ') {
//             value.push_back(c);
//             continue;
//         }

//         bi_data[row][column].o = stoi(value);
//         value.clear();

//         if (++column >= 30) {
//             column = 0;
//             row++;
//         }
//     }
// }

// void calc_weights() {
//     vector<int> v = string_to_vector("qwertyuiopasdfghjkl;zxcvbnm,./");
//     for(int i = 0; i < 30; i++) {
//         for(int j = 0; j < 30; j++) {
//             if (bi_data[i][j].speed == 0)
//                 continue;
//             double avg = double(bi_data[i][j].speed) / bi_data[i][j].o;
//             bi_weights[v[i]][v[j]] = avg;
//         }
//     }
// }

// void print_weights() {
//     for(int i = 0; i < 30; i++) {
//         for(int j = 0; j < 30; j++) {
//             cout << bi_weights[i][j] << " ";
//         }
//         cout << endl;
//     }
// }

// * * Type Test
void clear_print(const vector<string>& s) {
    system("cls");
    string output;
    for(uint i = 0; i < s.size(); i++) {
        output.append(s[i]);
        output.push_back('\n');
    }
    cout << output;
    cout.flush();
}

// x is the column, y is the row. The origin (0,0) is top-left.
void setCursorPosition(int y, int x)
{
    static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    std::cout.flush();
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hOut, coord);
}

void hidecursor()
{
   static const HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = FALSE;
   SetConsoleCursorInfo(consoleHandle, &info);
}

// * * Update Frame
void update_frame(const vector<string>& current, const vector<string>& previous) {
    if (previous.empty()) {
        clear_print(current);
        return;
    }
    for(uint i = 0; i < current.size(); i++) {
        for(uint j = 0; j < current[i].size(); j++) {
            if (previous[i][j] == current[i][j])
                continue;

            setCursorPosition(i, j);
            cout << current[i][j];

        }
        setCursorPosition(i, current[i].size());
        cout << string(100, ' ') << "\n";
    }
    cout.flush(); // updating the new data in terminal

}

struct Keystroke {
    int key;
    int speed;
    Keystroke(): key(-1), speed(0) {}
    Keystroke(int k, int s): key(k), speed(s) {}
};

void type_test(int line_length) {
    hidecursor();
    //read_weights();

    auto start = chrono::high_resolution_clock::now();
    auto before = start;
    Keystroke previous;
    Keystroke current;

    char c;
    bool wrong = false;
    uint index;
    vector<string> last_frame;
    vector<string> frame(2);
    while(true) {
        index = 0;
        frame[0] = (random_line_exact(line_length));
        while (index < frame[0].size()) {
            // Output
            frame[1] = (string(index, ' ') + "^");

            update_frame(frame, last_frame);
            last_frame = frame;

            // Timing and getting char
            if (!wrong)
                before = chrono::high_resolution_clock::now();
            c = getch();
            current.speed = since(before) / 1000;
            current.key = get_index(frame[0][index]);

            // Verifying char
            if (c == 3)
                return;
                //goto exit;
            if (c != frame[0][index]) {
                wrong = true;
                continue;
            }
            wrong = false;
            index++;

            // Data collection
            if (previous.key == -1) {
                previous = current;
                continue;
            }
            else if (current.key == -65) { // ' ' - 'a'
                previous.key = -1;
                continue;
            }
            //bi_data[previous.key][current.key].o++;
            //bi_data[previous.key][current.key].speed += (previous.speed + current.speed);

            previous = current;
        }
    }
    //exit:

    //calc_weights();
    //write_weights();
}


#endif