//remark:contracts: --contract_violation_information=none leaves the front end's constant-evaluation diagnostics alone: a contract violated in constant evaluation is still diagnosed at the assertion, with its user message
//type:fn
//options_all:--contracts_p3099 --contract_evaluation_semantic=quick_enforce --contract_violation_information=none
//match_regex:line 14: error: contract predicate is false in constant expression
//match_regex:line 22: error: contract predicate is false in constant expression
//match_regex:pre message
//match_regex:assert message
// Mirrors: gcc/testsuite/g++.dg/contracts/cpp26/contract-violation-information-none-constexpr.C
// (gnu_gcc) and clang/test/Contracts/contract-violation-information-none-constexpr.cpp
// (llvm_llvm-project), with quick_enforce rather than enforce so that it also runs
// under the C-generating back end.
constexpr int
pre_f(int x)
  pre(x > 0, "pre message")
{
  return x;
}

constexpr int
assert_f(int x)
{
  contract_assert(x > 0, "assert message");
  return x;
}

constexpr int a = pre_f(0);
constexpr int b = assert_f(0);
