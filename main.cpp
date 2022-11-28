#include"h/keyboard/keyboard.h"
#include"h/general/general.h"
#include"h/typing/typing.h"



int main() {
    srand(time(NULL));

    
    // * * Main keyboard evolution
    readIn();
    LOCKS.change_locks({});
    read_weights();
    calc_weights();
    

    Layout winner = multi(5000, 100);
    winner.print();
    

    //type_test(70);

    // print_weights();




}