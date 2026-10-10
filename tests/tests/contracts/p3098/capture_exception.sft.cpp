//remark:contracts: P3098: an exception from the initialization of a postcondition capture is a violation of kind post_capture (6), also with the C-generating back end: quick_enforce traps; under noexcept_observe the handler sees the evaluation_exception mode and the program continues, without checking the postcondition or destroying the capture
//options_all:--contracts_p3098 --contracts_p4298
//options_sep:!
//options:--contract_evaluation_semantic=quick_enforce -DHANDLER=0;rn!--contract_evaluation_semantic=noexcept_observe -DHANDLER=1;rp
//match_regex:^(start|end|kind 6 semantic 6 mode 2)$
// (No system headers: also runs under the C-generating back end, which
// links the contracts runtime, whose accessors the handler calls, only for
// the noexcept semantics.)
extern "C" int puts(const char *);
extern "C" int printf(const char *, ...);
extern "C" int fflush(void *);
extern "C" int stdc_contract_violation_kind(const void *);
extern "C" int stdc_contract_violation_semantic(const void *);
extern "C" int stdc_contract_violation_detection_mode(const void *);
namespace std { namespace contracts { class contract_violation; } }
#if HANDLER
void handle_contract_violation(const std::contracts::contract_violation &v) {
  printf("kind %d semantic %d mode %d\n", stdc_contract_violation_kind(&v),
         stdc_contract_violation_semantic(&v),
         stdc_contract_violation_detection_mode(&v));
  fflush(0);
}
#endif
void say(const char *s) { puts(s); fflush(0); }
struct Bad {
  Bad() {}
  Bad(const Bad &) { throw 1; }
  ~Bad() { say("capture destroyed"); }
};
bool checked() { say("postcondition checked"); return true; }
void f(const Bad &b) post [c = b] (checked()) { say("body"); }
int main() {
  say("start");
  Bad b;
  f(b);
  say("end");
}
