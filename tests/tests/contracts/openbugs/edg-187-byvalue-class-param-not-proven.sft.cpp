//remark:contracts: EDG-187: P4405 through edg-gxx: a precondition reading a member of a by-value class-type parameter is never proven at a call, even for a constant argument
//type:cn
//require:BACK_END_IS_CP_GEN_BE 1
//options:--c++26 --contracts --contracts_p3400 --contracts_p4405 -O2
//match_regex:Test_name.c:([0-9]+):[0-9]+: error: (contract assertion cannot be proven)
// Records the current (wrong) behavior: g++ (the back end of edg-gxx, which
// makes the analysis evaluations) cannot prove byval's precondition at
// either call, although the argument's member is the constant 1.  The same
// precondition on a const reference parameter (f3) or a scalar parameter
// (f4) is proven.  Expected: no error.  A fix shows up as a deviation.
#include <contracts>
using namespace std::contracts::labels;
struct T { int c; };
int byval(T t)        pre<always_prove>(t.c == 1);
int byref(const T& t) pre<always_prove>(t.c == 1);
int scalar(int c)     pre<always_prove>(c == 1);
int f1() { T a = {1}; return byval(a); }  // Bug, expected proven
int f2() { return byval(T{1}); }          // Bug, expected proven
int f3() { T a = {1}; return byref(a); }  // OK, proven
int f4() { return scalar(1); }            // OK, proven
