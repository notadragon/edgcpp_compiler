//remark:contracts: cpfe-cp parenthesizes a comma or assignment predicate
//require:BACK_END_IS_CP_GEN_BE 1
//type:rp
//options_all:--contract_evaluation_semantic=enforce
// The grammar makes a predicate a conditional-expression, so a comma or
// assignment must be put out within parentheses for g++ to accept it.
// Neither predicate is violated.
void f(int x) pre((x, true)) {}
void g(int *const p) pre((*p = 5)) { contract_assert((*p = 2)); }
int main() { int i = 0; f(1); g(&i); return i == 2 ? 0 : 1; }
