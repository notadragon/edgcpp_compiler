//remark:contracts: under --contract_violation_information=full the object g++ makes through edg-gxx holds each string object_none.sft.cpp requires to be absent under =none, so that test cannot pass for want of the text: the predicate's text, the user message, a label's computed comment and message, an implicit assertion's comment, <cassert>'s comment, the function and the file name
//require:BACK_END_IS_CP_GEN_BE 1
//type:cp
//options_sep:!
//options_all:--contracts_p3850 --contract_evaluation_semantic=observe --contract_group_evaluation_semantic=ub:expr.mul.div.by.zero:observe --contract_violation_information=full
//filter:tr '\0' '\n' < Test_name.o | grep -oE 'vi_(pre|assert|message|computed_comment|computed_message|file_name)_token|integer division by zero|[a-z] vi_[a-z_]*_function_token' | sort -u; echo done
//match_regex:^done$
//match_regex:^vi_pre_token$
//match_regex:^vi_assert_token$
//match_regex:^vi_message_token$
//match_regex:^vi_computed_comment_token$
//match_regex:^vi_computed_message_token$
//match_regex:^integer division by zero$
//match_regex:^vi_file_name_token$
//match_regex:^[a-z] vi_[a-z_]*_function_token$
// Mirrors: gcc/testsuite/g++.dg/contracts/cpp26/contract-violation-information-full-asm.C
// (gnu_gcc) and clang/test/Contracts/contract-violation-information-full-asm.cpp
// (llvm_llvm-project); the filter lists what it finds in the object instead
// of scanning the assembly.

#define __STDC_WANT_ASSERT_USES_CONTRACTS__ 1
#include <cassert>
#include <contracts>

struct vi_label_t {
  using assertion_control_object = vi_label_t;
  constexpr const char *compute_comment(const char *) const
  { return "vi_computed_comment_token"; }
  constexpr const char *compute_message(const char *) const
  { return "vi_computed_message_token"; }
};
constexpr vi_label_t vi_label{};

#line 1 "vi_file_name_token.C"
int vi_pre_function_token(int vi_pre_token)
  pre(vi_pre_token > 0, "vi_message_token")
  post(r : r != -1)
{
  contract_assert(vi_pre_token != 7);
  return vi_pre_token;
}
void vi_label_function_token(int x) pre<vi_label>(x > 0) { }
int vi_div_function_token(int a, int b) { return a / b; }
void vi_assert_function_token(int vi_assert_token)
{
  assert(vi_assert_token > 0);
}
