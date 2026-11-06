//remark:contracts: _Pre, _Post and _ContractAssert are ordinary identifiers without --contracts_p4299, or without contracts; with both they are keywords (as in Clang), and pre and post stay ordinary identifiers
//require:BACK_END_IS_CP_GEN_BE 1
//type:fp
//options:--c++26:--c++23 --contracts_p4299:--c++26 --contracts_p4299 --no_contracts_p4299:--c++26 --contracts_p4299
#if TEST_NUMBER <= 3
int _Pre = 1, _Post = 2, _ContractAssert = 3;
int g() { return _Pre + _Post + _ContractAssert; }
#else
int pre = 1, post = 2;
int g() { return pre + post; }
int f(int x) _Pre(x > 0) { _ContractAssert(x > 0); return x; }
#endif
