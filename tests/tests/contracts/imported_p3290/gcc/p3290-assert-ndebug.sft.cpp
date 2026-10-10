//remark: imported from gcc:p3290-assert-ndebug.C
//type: rp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts_p3290 -DNDEBUG -D__STDC_WANT_ASSERT_USES_CONTRACTS__
//use_system_includes: true
//linker_options: -lcontracts
// P3290: assert disabled with NDEBUG even if __STDC_WANT_ASSERT_USES_CONTRACTS__ is set.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts-p3290 -DNDEBUG -D__STDC_WANT_ASSERT_USES_CONTRACTS__" }

#include <cassert>

int main() {
  assert(1 == 2);  // Should be compiled out due to NDEBUG.
}
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }
