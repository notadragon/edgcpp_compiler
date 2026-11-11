//remark:contracts: under the noexcept semantics an exception from the evaluation of a predicate is reported to the violation handler with the evaluation_exception detection mode (2), also with the C-generating back end (EDG-7); noexcept_observe then continues, noexcept_enforce aborts, and a throwing handler terminates
//options_all:--contracts_p4298
//options_sep:!
//options:--contract_evaluation_semantic=noexcept_observe -DTHROWS=0;rp!--contract_evaluation_semantic=noexcept_enforce -DTHROWS=0;rn!--contract_evaluation_semantic=noexcept_observe -DTHROWS=1;rn
//match_regex:^(kind [123] semantic [67] mode 2|caught|end)$
// (No system headers: also runs under the C-generating back end.)  The
// handler reads the violation through libcontracts' C accessors.
extern "C" int puts(const char *);
extern "C" int printf(const char *, ...);
extern "C" int fflush(void *);
extern "C" int stdc_contract_violation_kind(const void *);
extern "C" int stdc_contract_violation_semantic(const void *);
extern "C" int stdc_contract_violation_detection_mode(const void *);
namespace std { namespace contracts { class contract_violation; } }

void handle_contract_violation(const std::contracts::contract_violation &v) {
  printf("kind %d semantic %d mode %d\n", stdc_contract_violation_kind(&v),
         stdc_contract_violation_semantic(&v),
         stdc_contract_violation_detection_mode(&v));
  fflush(0);
  if (THROWS) throw 1;
}

bool boom() { throw 3; }
int f(int x) pre (boom()) { return x; }
int g(int x) { contract_assert(boom()); return x; }
int h(const int x) post (boom()) { return x; }

int main() {
  try { f(1); g(2); h(3); } catch (...) { puts("caught"); }
  puts("end");
  return 0;
}
