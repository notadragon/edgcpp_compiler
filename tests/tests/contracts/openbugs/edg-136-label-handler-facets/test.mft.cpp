//remark:contracts: EDG-136: with the C-generating back end, a label's local_violation_label, compute_comment, compute_message and queryable_label facets, and a P3099 message, do not reach the violation handler
//require:DO_IL_LOWERING 1
//source_files:labels_stub.h
//options_all:--contracts_p3400 --contracts_p3099 --contracts_p4298 --contract_evaluation_semantic=noexcept_observe
//options:;rp
//match_regex:^(local handler|handler): .*$
// The recording is the current, wrong behavior.  Through edg-gxx (and with
// our GCC) the program prints
//   local handler: x > 0                   (f: logged's own handler runs)
//   handler: comment "x > 0", message "(none)", owner "(none)"
//   handler: comment "computed comment", message "computed message", ...
//   handler: comment "x > 0", message "plain message", owner "(none)"
//   handler: comment "x > 0", message "(none)", owner "review_committee"
// (with g++'s spelling of the comments); the C-generating back end passes
// only the comment, location, kind and semantic, so every line shows the
// predicate's text, no message and no owner, and logged's handler never runs.
#include "labels_stub.h"
extern "C" int printf(const char *, ...);
extern "C" int fflush(void *);
using std::contracts::contract_violation;

struct logged {
  using assertion_control_object = logged;
  static void handle_contract_violation(const contract_violation &v) {
    printf("local handler: %s\n", v.comment());
  }
};
constexpr logged logged_label;

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
  printf("handler: comment \"%s\", message \"%s\", owner \"%s\"\n",
         v.comment(), m ? m : "(none)", o ? o : "(none)");
  fflush(0);
}

void f(int x) pre<logged_label>(x > 0) { }
void g(int x) pre<annotated_label>(x > 0) { }
void h(int x) pre(x > 0, "plain message") { }
void k(int x) pre<owned_label>(x > 0) { }

int main() {
  f(0);
  g(0);
  h(0);
  k(0);
}
