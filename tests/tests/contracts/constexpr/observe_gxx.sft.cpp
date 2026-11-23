//remark:contracts: through edg-gxx an observed violation in constant evaluation is warned about by the front end and again by g++
//require:BACK_END_IS_CP_GEN_BE 1
//type:cp
//options_all:--contract_evaluation_semantic=observe
//match_regex:"Test_name.c", line 10: warning: contract predicate is false in constant expression
//match_regex:Test_name.c:10:[0-9]+: warning: contract predicate is false in constant expression
// g++ checks the contract assertions cpfe-cp puts out, and evaluates the
// same constant expression, so it warns a second time.  Both warnings are
// expected in this pairing (EDG-20, deferred).
constexpr int f(int x) pre(x > 0) { return x; }
constexpr int y = f(-1);
int g() { return y; }
