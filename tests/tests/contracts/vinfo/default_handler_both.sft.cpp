//remark:contracts: the default violation handler prints no empty location part or comment under --contract_violation_information=none, and all of them under =full, under both back ends; a violation that names only a line (the explicit P3290 C API) prints " at line N"
//options_all:--contracts_p4298 --contracts_p4299 --contract_evaluation_semantic=noexcept_observe
//type:rp
//options:--contract_violation_information=none:--contract_violation_information=full
//match_regex:^contract violation
// Mirrors: gcc/testsuite/gcc.dg/contracts/c-violation-information-default-handler{,-full}.c
// (gnu_gcc) and clang/test/Contracts/C/c-violation-information-default-handler{,-full}.c
// (llvm_llvm-project, which adds the explicit call), here in C++ (the suite
// cannot test C).  (No system headers: also runs under the C-generating back
// end.)
extern "C" void stdc_handle_observed_contract_violation_explicit(
    const char *, const char *, const char *, unsigned);

void pre_f(int x) _Pre(x > 0) { }
void assert_f(int x) { _ContractAssert(x > 0); }

int main() {
  pre_f(0);
  assert_f(0);
  stdc_handle_observed_contract_violation_explicit("only a line", "", "", 7);
  return 0;
}
