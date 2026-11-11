//remark:contracts: with the C-generating back end, under the noexcept semantics, a violation's record carries what a label adds -- its local violation handler (which can claim the violation), its computed comment and message, its query function -- and a P3099 message, for a false predicate and for an evaluation exception; under --contract_violation_information=none only the handler and the query function
//require:DO_IL_LOWERING 1
//source_files:labels_stub.h
//options_all:--contracts_p3400 --contracts_p3099 --contracts_p4298 --contract_evaluation_semantic=noexcept_observe
//options_sep:!
//options:;rp!--contract_violation_information=none;rp
//match_regex:^(local handler|handler|claimed|end).*$
// Mirrors (through edg-gxx, where g++ builds the records):
// gcc/testsuite/g++.dg/contracts/cpp26/p3400-facet-local-handler.C,
// p3400-facet-message.C, p3400-facet-query-basic.C and
// p3099-message-basic.C (gnu_gcc).
#include "labels_stub.h"
extern "C" int printf(const char *, ...);
extern "C" int fflush(void *);
extern "C" int stdc_contract_violation_detection_mode(const void *);
using std::contracts::contract_violation;
using std::contracts::violation_handled;

struct logged {
  using assertion_control_object = logged;
  static void handle_contract_violation(const contract_violation &v) {
    printf("local handler: \"%s\", mode %d\n", v.comment(),
           stdc_contract_violation_detection_mode(&v));
  }
};
constexpr logged logged_label;

struct claiming {
  using assertion_control_object = claiming;
  violation_handled handle_contract_violation(const contract_violation &)
      const {
    printf("claimed\n");
    return violation_handled::handled;
  }
};
constexpr claiming claiming_label;

struct annotated {
  using assertion_control_object = annotated;
  constexpr const char *compute_comment(const char *) const {
    return "computed comment";
  }
  constexpr const char *compute_message(const char *m) const {
    return m ? m : "computed message";
  }
};
constexpr annotated annotated_label;

struct owned {
  using assertion_control_object = owned;
  static constexpr int key = 1;
  void *query(const void *k, std::size_t index) const {
    return k == &key && index == 0 ? (void *)"review_committee" : nullptr;
  }
};
constexpr owned owned_label;

void handle_contract_violation(const contract_violation &v) {
  const char *m = v.message();
  const char *o = (const char *)v.query_control_object(&owned::key, 0);
  printf("handler: comment \"%s\", message \"%s\", owner \"%s\", mode %d\n",
         v.comment(), m ? m : "(none)", o ? o : "(none)",
         stdc_contract_violation_detection_mode(&v));
  fflush(0);
}

bool throws() { throw 1; }

void f(int x) pre<logged_label>(x > 0) { }
void g(int x) pre<annotated_label>(x > 0) { }
void h(int x) pre(x > 0, "plain message") { }
void k(int x) pre<owned_label>(x > 0) { }
void c(int x) pre<claiming_label>(x > 0) { }
void e(int) pre<logged_label>(throws()) { }
void m(int x) { contract_assert<annotated_label>(x > 0, "given message"); }

int main() {
  f(0);
  g(0);
  h(0);
  k(0);
  c(0);
  e(0);
  m(0);
  printf("end\n");
}
