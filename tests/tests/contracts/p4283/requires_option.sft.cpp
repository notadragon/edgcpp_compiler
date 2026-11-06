//remark:contracts: without P4283 a requires-clause on a contract assertion is diagnosed and skipped, and __cpp_contracts_requires is not defined
//type:fn
//options_all:--contract_evaluation_semantic=quick_enforce
//match_regex:line 9: error: requires clause on contract assertions requires --contracts_p4283
//match_regex:line 10: error: requires clause on contract assertions requires --contracts_p4283
//match_regex:line 11: error: requires clause on contract assertions requires --contracts_p4283
//match_regex:line 14: error: static assertion failed with "P4283 is off"
template <class T> concept Any = true;
template <class T> void a(T x) pre requires Any<T> (x > 0);          // Error
template <class T> T b(const T x) post requires (Any<T>) (r: r > x); // Error
template <class T> void c(T x) { contract_assert requires Any<T> (x > 0); }  // Error
template void c<int>(int);
#ifndef __cpp_contracts_requires
static_assert(false, "P4283 is off");   // Error
#endif
