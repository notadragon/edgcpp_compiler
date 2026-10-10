//remark:contracts: quick_enforce checks of functions declared in a precompiled header
//source_files:header.h
//options_all:--pch --pch_test_mode --contract_evaluation_semantic=quick_enforce
//options:-DVIOLATE=0;rp:-DVIOLATE=1;rn:-DVIOLATE=2;rn:-DVIOLATE=3;rn:-DVIOLATE=4;rn:-DVIOLATE=5;rn:-DVIOLATE=6;rn
// The second compilation uses the PCH file the first one created; t is
// instantiated only after the PCH.  Also runs under the C-generating back
// end (no system headers there).
#include "header.h"

int main() {
  if (VIOLATE == 0) { f(2); g(0); t(1); M().m(1); lam(1); }
  if (VIOLATE == 1) f(0);         // precondition
  if (VIOLATE == 2) f(1);         // postcondition
  if (VIOLATE == 3) g(3);         // contract_assert
  if (VIOLATE == 4) t(0L);        // template's precondition
  if (VIOLATE == 5) M().m(0);     // member's precondition
  if (VIOLATE == 6) lam(0);       // lambda's precondition
  puts("end");
}
