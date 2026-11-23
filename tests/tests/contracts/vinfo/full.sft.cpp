//remark:contracts: under --contract_violation_information=full, the default, every contract-violation record g++ builds through edg-gxx names the file, function, line and column and carries the comment and message, for the cases of none.sft.cpp, and __gcc_contracts_violation_information_none is not predefined
//require:BACK_END_IS_CP_GEN_BE 1
//type:rp
//options_sep:!
//options_all:--contracts_p3850 --contract_evaluation_semantic=observe --contract_group_evaluation_semantic=ub:expr.mul.div.by.zero:observe
//options:!--contract_violation_information=full
//match_regex:^end$
// Mirrors: gcc/testsuite/g++.dg/contracts/cpp26/contract-violation-information-full.C
// (gnu_gcc) and clang/test/Contracts/Runnable/contract-violation-information-full.cpp
// (llvm_llvm-project); the first case gives no option (the default).
// The comment of a predicate is g++'s printing of the generated C++
// ("(((int)((const int)x)) > 0)"), hence strstr; the function names are
// g++'s too.

#ifdef __gcc_contracts_violation_information_none
#error "__gcc_contracts_violation_information_none is defined"
#endif

#define __STDC_WANT_ASSERT_USES_CONTRACTS__ 1
#include <cassert>
#include <contracts>
#include <cstdio>
#include <cstdlib>
#include <cstring>

using namespace std::contracts;

static int count;
static int local_count;
static assertion_kind expected_kind;
static const char *expected_function;
static unsigned expected_line;
static const char *expected_comment;
static const char *expected_message;

static void check_full(const contract_violation &v) {
  if (std::strcmp(v.location().file_name(), __FILE__) != 0 ||
      !std::strstr(v.location().function_name(), expected_function) ||
      v.location().line() != expected_line ||
      v.location().column() == 0 ||
      !std::strstr(v.comment(), expected_comment))
    __builtin_abort();
  if (expected_message
      ? !v.message() || std::strcmp(v.message(), expected_message) != 0
      : v.message() != nullptr)
    __builtin_abort();
  if (v.kind() != expected_kind ||
      v.detection_mode() != detection_mode::predicate_false)
    __builtin_abort();
}

void handle_contract_violation(const contract_violation &v) {
  check_full(v);
  if (v.kind() == assertion_kind::cassert) {
    if (v.semantic() != evaluation_semantic::enforce) __builtin_abort();
    if (count != 6 || local_count != 1) __builtin_abort();
    std::puts("end");
    std::fflush(stdout);
    std::_Exit(0);
  }
  if (v.semantic() != evaluation_semantic::observe) __builtin_abort();
  ++count;
}

struct vi_label_t {
  using assertion_control_object = vi_label_t;
  constexpr const char *compute_comment(const char *) const
  { return "computed comment"; }
  constexpr const char *compute_message(const char *) const
  { return "computed message"; }
  void handle_contract_violation(const contract_violation &v) const {
    check_full(v);
    ++local_count;
  }
};
constexpr vi_label_t vi_label{};

int pre_f(int x) pre(x > 0) { return x; }  const unsigned pre_line = __LINE__;
int post_f(int x) post(r : r > 0) { return x; }  const unsigned post_line = __LINE__;
void assert_f(int x) { contract_assert(x > 0); }  const unsigned assert_line = __LINE__;
int div_f(int a, int b) { return a / b; }  const unsigned div_line = __LINE__;
void msg_f(int x) pre(x > 0, "a user message") { }  const unsigned msg_line = __LINE__;
void label_f(int x) pre<vi_label>(x > 0) { }  const unsigned label_line = __LINE__;

static void expect(assertion_kind kind, const char *function, unsigned line,
                   const char *comment, const char *message) {
  expected_kind = kind;
  expected_function = function;
  expected_line = line;
  expected_comment = comment;
  expected_message = message;
}

int main() {
  expect(assertion_kind::pre, "pre_f", pre_line, "> 0", nullptr);
  pre_f(0);
  expect(assertion_kind::post, "post_f", post_line, "> 0", nullptr);
  post_f(0);
  expect(assertion_kind::assert, "assert_f", assert_line, "> 0", nullptr);
  assert_f(0);
  expect(assertion_kind::implicit, "div_f", div_line,
         "integer division by zero", nullptr);
  div_f(1, 0);
  expect(assertion_kind::pre, "msg_f", msg_line, "> 0", "a user message");
  msg_f(0);
  expect(assertion_kind::pre, "label_f", label_line, "computed comment",
         "computed message");
  label_f(0);
  if (count != 6 || local_count != 1) __builtin_abort();
  expect(assertion_kind::cassert, "main", __LINE__ + 2, "zero == 1", nullptr);
  int zero = 0;
  assert(zero == 1);
  __builtin_abort();
}
