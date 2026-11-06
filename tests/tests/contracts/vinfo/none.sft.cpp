//remark:contracts: under --contract_violation_information=none every contract-violation record g++ builds through edg-gxx is empty (file, function, line, column, comment, message) for pre, post, contract_assert, an implicit (P3100) assertion, a user message (P3099), a label's computed comment and message (P3400) and <cassert>'s assert (P3290); kind, semantic, detection mode and the local handler are unchanged, and __gcc_contracts_violation_information_none is predefined
//require:BACK_END_IS_CP_GEN_BE 1
//type:rp
//options_sep:!
//options_all:--contracts_p3850 --contract_evaluation_semantic=observe --contract_group_evaluation_semantic=ub:expr.mul.div.by.zero:observe --contract_violation_information=none
//match_regex:^end$
// Mirrors: gcc/testsuite/g++.dg/contracts/cpp26/contract-violation-information-none.C
// (gnu_gcc) and clang/test/Contracts/Runnable/contract-violation-information-none.cpp
// (llvm_llvm-project).  full.sft.cpp is the same program under =full.  The
// front end predefines the macro (it preprocesses for g++), so libstdc++'s
// <cassert> passes an empty location and comment.

#ifndef __gcc_contracts_violation_information_none
#error "__gcc_contracts_violation_information_none is not defined"
#endif

#define __STDC_WANT_ASSERT_USES_CONTRACTS__ 1
#include <cassert>
#include <contracts>
#include <cstdio>
#include <cstdlib>

using namespace std::contracts;

static int count;
static int local_count;
static assertion_kind expected_kind;
static detection_mode expected_mode = detection_mode::predicate_false;

static void check_empty(const contract_violation &v) {
  if (v.location().file_name()[0] != '\0' ||
      v.location().function_name()[0] != '\0' ||
      v.location().line() != 0 ||
      v.location().column() != 0 ||
      v.comment()[0] != '\0' ||
      v.message() != nullptr)
    __builtin_abort();
  if (v.kind() != expected_kind || v.detection_mode() != expected_mode)
    __builtin_abort();
}

void handle_contract_violation(const contract_violation &v) {
  check_empty(v);
  if (v.kind() == assertion_kind::cassert) {
    // The <cassert> path enforces: it terminates after the handler.
    if (v.semantic() != evaluation_semantic::enforce) __builtin_abort();
    if (count != 6 || local_count != 1) __builtin_abort();
    std::puts("end");
    std::fflush(stdout);
    std::_Exit(0);
  }
  if (v.semantic() != evaluation_semantic::observe) __builtin_abort();
  ++count;
}

// A label whose computed comment and message would otherwise be recorded,
// with a local handler that still runs.
struct vi_label_t {
  using assertion_control_object = vi_label_t;
  constexpr const char *compute_comment(const char *) const
  { return "computed comment"; }
  constexpr const char *compute_message(const char *) const
  { return "computed message"; }
  void handle_contract_violation(const contract_violation &v) const {
    check_empty(v);
    ++local_count;
  }
};
constexpr vi_label_t vi_label{};

int pre_f(int x) pre(x > 0) { return x; }
int post_f(int x) post(r : r > 0) { return x; }
void assert_f(int x) { contract_assert(x > 0); }
int div_f(int a, int b) { return a / b; }
void msg_f(int x) pre(x > 0, "a user message") { }
void label_f(int x) pre<vi_label>(x > 0) { }

int main() {
  expected_kind = assertion_kind::pre;
  pre_f(0);
  expected_kind = assertion_kind::post;
  post_f(0);
  expected_kind = assertion_kind::assert;
  assert_f(0);
  expected_kind = assertion_kind::implicit;
  div_f(1, 0);
  expected_kind = assertion_kind::pre;
  msg_f(0);
  label_f(0);
  if (count != 6 || local_count != 1) __builtin_abort();
  expected_kind = assertion_kind::cassert;
  int zero = 0;
  assert(zero == 1);
  __builtin_abort();
}
