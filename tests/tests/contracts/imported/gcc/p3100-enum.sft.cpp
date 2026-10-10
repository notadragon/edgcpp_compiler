//remark: imported from gcc:p3100-enum.C
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts
// P3100: the "assume" evaluation_semantic enumerator is available
// unconditionally in <contracts> (no -fcontracts-p3100 required), so a
// violation handler can name it without preprocessor guards.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts" }
// { dg-skip-if "requires hosted libstdc++" { ! hostedlib } }

#include <contracts>

static_assert(static_cast<int>(std::contracts::evaluation_semantic::assume)
	      == 5, "assume must have value 5");
