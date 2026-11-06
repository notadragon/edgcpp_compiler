//remark:contracts: with the C-generating back end, under --contract_violation_information=none the object holds none of the text of the contract-violation records its checks build (the predicate's text, the function and the file name), and under =full it holds each of them
//require:DO_IL_LOWERING 1
//type:cp
//options_all:--contracts_p4298 --contracts_p4299 --contract_evaluation_semantic=noexcept_observe
//options:--contract_violation_information=none:--contract_violation_information=full
//filter:tr '\0' '\n' < Test_name.o | grep -oE 'vi_(pre|post|assert|c_pre)_token|vi_file_name_token|[a-z] vi_[a-z_]*_function_token' | sort -u; echo done
//match_regex:^done$
// Mirrors: gcc/testsuite/g++.dg/contracts/cpp26/contract-violation-information-{none,full}-asm.C
// and gcc/testsuite/gcc.dg/contracts/c-violation-information-{none,full}-asm.c
// (gnu_gcc), for what the C-generating back end's checks record (no user
// message, label or implicit assertion reaches them).  The file name is a
// distinctive #line name.
#line 1 "vi_file_name_token.C"
enum { vi_post_token = 1 };
int vi_pre_function_token(int vi_pre_token)
  pre(vi_pre_token > 0)
  post(r : r != -vi_post_token)
{
  contract_assert(vi_pre_token != 7);
  return vi_pre_token;
}
void vi_assert_function_token(int vi_assert_token)
{
  contract_assert(vi_assert_token > 0);
}
int vi_c_function_token(int vi_c_pre_token) _Pre(vi_c_pre_token > 0)
{
  return vi_c_pre_token;
}
