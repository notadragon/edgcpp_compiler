//remark:contracts: P3097 with the noexcept semantics: a violation of the statically chosen function's precondition in a virtual call reports that function, also with the C-generating back end (EDG-71)
//options_all:--contracts_p3097 --contracts_p4298 --contract_evaluation_semantic=noexcept_observe
//options:;rp
//match_regex:^(kind 1 .*|end)$
// (No system headers: also runs under the C-generating back end.)
extern "C" int puts(const char *);
extern "C" int printf(const char *, ...);
extern "C" int fflush(void *);
extern "C" int stdc_contract_violation_kind(const void *);
extern "C" const char *stdc_contract_violation_function(const void *);
extern "C" const char *stdc_contract_violation_comment(const void *);
namespace std { namespace contracts { class contract_violation; } }
void handle_contract_violation(const std::contracts::contract_violation &v) {
  printf("kind %d function %s comment %s\n", stdc_contract_violation_kind(&v),
         stdc_contract_violation_function(&v),
         stdc_contract_violation_comment(&v));
  fflush(0);
}
struct B { virtual int f(int x) pre (x > 0) { return x; } virtual ~B() {} };
struct D : B { int f(int x) override { return x; } };
int main() {
  D d; B *p = &d;
  p->f(0);
  puts("end");
}
