#include"h/keyboard/keyboard.h"
#include"h/general/general.h"
//#include"h/typing/typing.h"

/*
To-do/notes:
I honestly feel that's it is pretty optimal at this point excluding a few things
    I'm not super confident on how the different outward rolls are weighted
    Might be a good idea to filter the reddit data some more
    Could improve the evolution techniques for more consistent results. But I'm not even sure that would change much because the last two results I got were the same
    Could re-implement quads, but I think it's just too slow plus there really isn't anything a quad can detect that a triple can't.
    Hard to implement, but I factor in the fact that I usually hit the 'u' key with my middle finger.

FINALLY, solved the issue of why my bigram calculations have been so off. I was deciding whether a bigram just hitting the same key by checking the row of the key, but that misses out on the pointer finger.
Obviously the pointer finger has annoying bigrams that are on the same row, unlike the other keys.

*/


double double_bigrams(const vector<int> v) {
    int total = 0;
    //int same = 0;
    int count = 0;
    vector<Letter> letters(3);
    for(uint i = 0; i < TRIPLES.size(); i++) {
        letters[0] = Letter(v[TRIPLES[i].word[0]]);
        letters[1] = Letter(v[TRIPLES[i].word[1]]);
        letters[2] = Letter(v[TRIPLES[i].word[2]]);
        if (letters[0].finger == letters[1].finger && letters[0].position != 30 && letters[1].position != 30) {
            if (letters[0].position == letters[1].position)
                continue;
            count += TRIPLES[i].count;
        }
        if (letters[1].finger == letters[2].finger && letters[1].position != 30 && letters[2].position != 30) {
            if (letters[1].position == letters[2].position)
                continue;
            count += TRIPLES[i].count;
        }
        total += 2 * TRIPLES[i].count;
    }
    return count / double(total);
}

int main() {
    srand(time(NULL));

    
    // * * Main keyboard evolution
    readIn();
    LOCKS.change_locks({});
    get_scores();

    // * * Evolve
    Layout top = multi(200, 200, 100);


    print_data(string_to_vector("qwfpbjluy;arstgmneiozxcdvkh,./"));
    print_data(top);

    // * * Equivalents
    // string strong = "mualbzwypdriongcseth;.,xqvf/kj";;
    // vector<int> v = string_to_vector(strong);
    // Layout layout(v);
    // layout.score = fitness(v);

    // vector<Layout> equiv = equivalents(layout, 50);
    // for(uint i = equiv.size() - 1; i >= 0; i--) {
    //     cout << (int)equiv[i].score << endl;
    //     print_keyboard(equiv[i]);
    // }

    // * * Same-finger
    // string good = "gldfbz.oiyhrtspunaeckxmwv;,q/j";

    // print_data(string_to_vector(good));
    // cout << double_bigrams(string_to_vector(good)) << endl;
    // print_data(string_to_vector("qwfpbjluy;arstgmneiozxcdvkh,./"));
    // cout << double_bigrams(string_to_vector("qwfpbjluy;arstgmneiozxcdvkh,./")) << endl;
}