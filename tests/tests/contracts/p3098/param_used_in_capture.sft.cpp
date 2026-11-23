//remark:contracts: P3098: a parameter named only in a capture initializer is used: no "set but never used" warning when the body assigns to it (EDG-74)
//require:BACK_END_IS_CP_GEN_BE 1
//type:fp
//options_all:--contracts_p3098
int f(int i) post [j = i] (r: r != j) { i = 2; return 0; }
int g(int i) post [i] (r: r != i) { i = 2; return 0; }
