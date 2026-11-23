//remark:contracts: quick_enforce checks of eight function templates declared in a precompiled header and instantiated after it
//source_files:header.h
//options_all:--pch --pch_test_mode --contract_evaluation_semantic=quick_enforce
//options:-DVIOLATE=0;rp:-DVIOLATE=1;rn:-DVIOLATE=2;rn:-DVIOLATE=3;rn
// Mirror of the templates in GCC's g++.dg/pch/contract-runtime.C (GCC-646;
// pch/runtime has one template): the second compilation uses the PCH the
// first one created, and each template is instantiated only after it.
// Also runs under the C-generating back end (no system headers there).
#include "header.h"

int main() {
  int s = t0(1) + t1(2) + t2(3) + t3(4) + t4(5) + t5(6) + t6(7) + t7(8);
  if (VIOLATE == 1) s += t7(7);     // t7's precondition
  if (VIOLATE == 2) s += t3(12L);   // t3's postcondition, a second instance
  if (VIOLATE == 3) s += t0(0);     // t0's precondition
  if (s > 0) puts("end");
}
