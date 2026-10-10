//remark:contracts: EDG-108: a fold over a function parameter pack in the postcondition of a non-defining declaration of a function template with a deduced return type and a result name fails to instantiate
//require:BACK_END_IS_CP_GEN_BE 1
//type:fn
// The recording is the current, wrong behavior: when this test deviates,
// the bug may be fixed.  Correct: no error.  "fold expression does not
// refer to a parameter pack" at the first declaration; without the result
// name, with a declared return type, or with only the definition, it
// compiles.  From GCC's p3098-capture-deduced-return.C (EDG-96).
template <class... T> auto g(const T... t) post(r: ((t > 0) && ...));
template <class... T> auto g(const T... t) post(r: ((t > 0) && ...)) { return 0; }
int main() { return g(1, 2); }
