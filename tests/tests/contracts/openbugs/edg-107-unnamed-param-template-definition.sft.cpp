//remark:contracts: EDG-107: a function template's definition that leaves a parameter a postcondition odr-uses unnamed and not const is not diagnosed
//require:BACK_END_IS_CP_GEN_BE 1
//type:fp
// The recording is the current, wrong behavior: when this test deviates,
// the bug may be fixed.  Correct: an error at t's definition, as when it
// names the parameter or is a non-defining redeclaration.  From
// Clang's postcondition-unnamed-param-diagnostic.cpp (EDG-95).
template <class T> void t(const T b) post(b > 0);
template <class T> void t(T) {}                     // Error, not given
template void t<int>(int);
