//remark:contracts: a definition's unnamed parameters that a postcondition odr-uses must be const
//type:fn
//match_regex:line 13: error: an unnamed value parameter used in a postcondition must be declared const
//match_regex:line 17: error: an unnamed value parameter used in a postcondition must be declared const
//match_regex:line 19: error: an unnamed value parameter used in a postcondition must be declared const
//match_regex:line 21: error: an unnamed value parameter used in a postcondition must be declared const
//match_regex:line 24: error: an unnamed value parameter used in a postcondition must be declared const
//match_regex:line 29: error: an unnamed value parameter used in a postcondition must be declared const
//match_regex:line 32: error: an unnamed value parameter used in a postcondition must be declared const
// [dcl.contract.func]: also on a definition that leaves the parameter
// unnamed, as GCC and Clang diagnose (EDG-107).
void r1(const int b) post(b > 0);
void r1(int) {}                                  // Error
void r2(const int b) post(b > 0);
void r2(const int) {}                            // OK
void r3(const int a, const int b) post(b > 0);
void r3(int, int) {}                             // Error, the second only
void r4(const int b) post(b > 0);
void r4(int) try {} catch (...) {}               // Error
template <class T> void t1(const T b) post(b > 0);
template <class T> void t1(T) {}                 // Error in t1<int>
template void t1<int>(int);
template <class T> void t2(const int b) post(b > 0);
template <class T> void t2(int) {}               // Error, not dependent
struct S {
  void m(const int b) post(b > 0);
  static void s(const int b) post(b > 0);
};
void S::m(int) {}                                // Error
void S::s(const int) {}                          // OK
int r5(const int b) post(r: r > b);
int r5(int) { return 1; }                        // Error
template <class T> void t3(const T b) post(b > 0);
template <class T> void t3(T) {}
template void t3<const int>(int);                // OK
