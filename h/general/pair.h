#ifndef __PAIR_CLASS__
#define __PAIR_CLASS__

template<class A, class B>
class Pair {
public:
    A a;
    B b;
    Pair() {}
    Pair(A arg1): a(arg1) {}
    Pair(A arg1, B arg2): a(arg1), b(arg2) {}
};

template<class A, class B>
bool pair_compare_a(const Pair<A,B>& a, const Pair<A,B>& b) {
    return a.a > b.a;
}
template<class A, class B>
bool pair_compare_b(const Pair<A,B>& a, const Pair<A,B>& b) {
    return a.b > b.b;
}

template<class A, class B>
bool pair_compare_a_reverse(const Pair<A,B>& a, const Pair<A,B>& b) {
    return a.a < b.a;
}
template<class A, class B>
bool pair_compare_b_reverse(const Pair<A,B>& a, const Pair<A,B>& b) {
    return a.b < b.b;
}





#endif