#include"h/keyboard/keyboard.h"
#include"h/general/general.h"
//#include"h/typing/typing.h"



int main() {
    srand(time(NULL));

    
    // * * Main keyboard evolution
    readIn();
    LOCKS.change_locks({});
    // read_weights();
    // calc_weights();
    

    // Layout winner = multi(5000, 100);
    // winner.print();
    

    //type_test(70);

    // print_weights();

    //cout << "locks" << endl;
    //anecdotal_weights();

    // Letter test(0);
    // cout << test.finger << endl;
    // Letter tes(10);
    // cout << tes.finger << endl;
    // Letter te(20);
    // cout << te.finger << endl;

    anecdotal_weights(string_to_vector("hrstg;nieylmcdw,.aopqxvkzjbu/f"));


    // cout << ALL_QUADGRAM_WEIGHTS[5][3][9][10] << endl;
    // cout << ALL_QUADGRAM_WEIGHTS[0][10][20][1] << endl;
    // cout << singles[4] << endl;
    // cout << singles[13] << endl;
    // cout << singles[28] << endl;
    // cout << "weighs" << endl;

    // cout << "evolving" << endl;
    // Layout l = multi(10, 50, 50);
    // l.print();
    
    // vector<int> layout(string_to_vector("wlypbzfou'crstgmneiaqjvdkxh/,."));
    // layout.push_back(30);
    // cout << (int)fitness(layout) << endl;
    //cout << (int)fitness(string_to_vector("flhvzqwuoysrntkcdeaix;bmjpg,./")) << endl;




}