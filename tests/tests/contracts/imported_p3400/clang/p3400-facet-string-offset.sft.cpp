//remark: imported from clang:p3400-facet-string-offset.cpp
//type: rp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3400 --contracts_p3099 --contract_evaluation_semantic=observe
//use_system_includes: true
//linker_options: -lcontracts
// EDG: adapted -- the comment is formed from the predicate cpfe-cp puts
// out (implicit conversions shown), which varies conformingly (DECISIONS.md
// E10): skip1's comment is compared with plain's past its first character.
// RUN: %clangxx -std=c++26 %s -fcontracts -fcontracts-p3400 -fcontracts-p3099 -fcontract-evaluation-semantic=observe %libcxx_flags -o %t && %t

// P3400: a compute_comment / compute_message facet may return a pointer into
// the string it was given at an offset; the string is read through the
// returned pointer, offset included.
//
// GCC: gcc/testsuite/g++.dg/contracts/cpp26/p3400-facet-string-offset.C

#include <contracts>
#include <cstring>

static const char *comment, *message;

void handle_contract_violation(const std::contracts::contract_violation &v) {
  comment = v.comment();
  message = v.message();
}

static bool is(const char *p, const char *s) { return p && !std::strcmp(p, s); }

struct skip1_t {
  using assertion_control_object = skip1_t;
  constexpr const char *compute_comment(const char *c) const { return c ? c + 1 : c; }
  constexpr const char *compute_message(const char *m) const { return m ? m + 1 : m; }
};
constexpr skip1_t skip1{};

struct plus1_t {
  using assertion_control_object = plus1_t;
  constexpr const char *compute_message(const char *m) const { return m + 1; }
};
constexpr plus1_t plus1{};

void plain(int x) pre(x > 0) {}
void f(int x) pre<skip1>(x > 0, "xmsg") {}
void g(int x) pre<plus1>(x > 0, "xmsg") {}

int main() {
  plain(0);
  const char *plain_comment = comment;
  if (!plain_comment || !*plain_comment)
    __builtin_abort();
  f(0);
  if (!is(comment, plain_comment + 1) || !is(message, "msg"))
    __builtin_abort();
  g(0);
  if (!is(message, "msg"))
    __builtin_abort();
}

// REQUIRES: contracts-libcxx, native
