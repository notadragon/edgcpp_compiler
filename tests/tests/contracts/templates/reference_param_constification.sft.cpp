//remark:contracts: a template parameter of reference type is const in a predicate
//type:fn
//require:BACK_END_IS_CP_GEN_BE 1
//match_regex:", line 7: error
//match_regex:", line 8: error
// R is declared outside the assertion and has reference type: const.
template <int &R> void t1() pre(++R > 0) {}                     // Error
template <int &R> void t2() { contract_assert(++R > 0); }       // Error
template <int N, int *P> void t3() pre(++*P > N) {}             // OK
int g;
template void t1<g>();
template void t2<g>();
template void t3<1, &g>();
