//remark:contracts: the elements of a parameter pack odr-used in a postcondition must be const
//type:fn
//match_regex:line 13: error: value parameter "a" used in a postcondition must be declared const
//match_regex:line 14: error: value parameter "a" used in a postcondition must be declared const
//match_regex:line 15: error: value parameter "a" used in a postcondition must be declared const
//match_regex:line 16: error: value parameter "x" used in a postcondition must be declared const
//match_regex:line 21: error: value parameter "b" used in a postcondition must be declared const
// [dcl.contract.func]: each element of an expanded parameter pack that a
// postcondition odr-uses is a parameter that must be const; with pack
// indexing, only the element selected.  Each error is issued once, also for
// an explicit instantiation and a member of a class template.  Lines 23 to
// 28 are OK.
template <typename... Ts> void pi(Ts... a) post(a...[0] == 0) {}
template <typename... Ts> void fold(Ts... a) post((... && (a == 0))) {}
template <typename T> void one(T a) post(a == 0) {}
template <class T> struct S { void m(T x) post(x > 0) {} };
template void pi<int>(int);
template void fold<int>(int);
template void one<int>(int);
void use_m() { S<int>{}.m(1); }
template <typename... Ts> void sel_nc(Ts... b) post(b...[0] == 0) {}
template void sel_nc<int, const int>(int, const int);
template <typename... Ts> void sel_c(Ts... c) post(c...[1] == 0) {}
template void sel_c<int, const int>(int, const int);
template <typename... Ts> void all_c(const Ts... d) post(d...[0] == 0) {}
template void all_c<int>(const int);
template <typename... Ts> void disc(Ts... e) post((((void)e, ...), true)) {}
template void disc<int, int>(int, int);
