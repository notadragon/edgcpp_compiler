//remark:contracts: a violation under enforce terminates the program
//require:BACK_END_IS_CP_GEN_BE 1
//type:rn
//options_all:--contract_evaluation_semantic=enforce
//cases:3
#include <cstdio>

int f(int x) pre(x > 0) post(r: r > 1) { return x; }

int main() {
  std::fprintf(stderr, "start\n");
#if TEST_NUMBER == 1
  f(0);                        // precondition
#elif TEST_NUMBER == 2
  f(1);                        // postcondition
#else
  contract_assert(TEST_NUMBER < 3);
#endif
  std::fprintf(stderr, "not reached\n");
}
