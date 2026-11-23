//remark: imported from gcc:violation-handler-constexpr.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contract_evaluation_semantic=observe
//match_regex: ", line 19: (?:catastrophic )?error
// Mirror: clang/test/Contracts/violation-handler-constexpr.cpp in the
// llvm_llvm-project fork (GCC-951).  EDG rejects the constexpr handler as
// implicitly inline.
// A constexpr ::handle_contract_violation is implicitly inline
// ([dcl.constexpr]), so it cannot replace the violation handler
// ([dcl.fct.def.replace]) and is diagnosed at its declaration.  The
// consteval spelling is pr128202.C's.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontract-evaluation-semantic=observe" }
// { dg-skip-if "requires hosted libstdc++" { ! hostedlib } }

#include <contracts>

constexpr void handle_contract_violation (const std::contracts::contract_violation &) {} // { dg-error "'::handle_contract_violation' must not be declared 'inline'" }

int main ()
{
  contract_assert (false);
}
