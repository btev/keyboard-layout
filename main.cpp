#include"h/keyboard/keyboard.h"
#include"h/general/general.h"



int main() {
    srand(time(NULL));
    readIn();
    LOCKS.change_locks({});

    Layout winner = multi(5000, 100);
    winner.print();
}