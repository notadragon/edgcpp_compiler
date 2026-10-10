//remark:contracts: EDG-109: --contracts_p4299 in C++ predefines no vendor macro (our GCC defines __gcc_contracts_p4299, Clang __clang_contracts_p4299)
//require:BACK_END_IS_CP_GEN_BE 1
//type:fn
//options:--contracts_p4299
// The recording is the current, wrong behavior: when this test deviates,
// the bug may be fixed.  Correct: the macro is defined and positive (the
// implemented revision's date), and not without --contracts_p4299.  From
// GCC's p4299-feature-macro.C (EDG-98).
static_assert(__gcc_contracts_p4299 > 0);   // Error, not defined
