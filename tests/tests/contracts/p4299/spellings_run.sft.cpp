//remark:contracts: with --contracts_p4299, _Pre, _Post and _ContractAssert are pre, post and contract_assert, each checked: on functions, members, templates, lambdas and with a P3400 label, under both back ends
//options_all:--contracts_p4299 --contracts_p3400 --contracts_p4298 --contract_evaluation_semantic=noexcept_observe
//type:rp
//match_regex:^(kind [123] line [0-9]+|end)$
// (No system headers: also runs under the C-generating back end.)  The
// handler reads the violation through libcontracts' C accessors.
extern "C" int puts(const char *);
extern "C" int printf(const char *, ...);
extern "C" int fflush(void *);
extern "C" int stdc_contract_violation_kind(const void *);
extern "C" unsigned stdc_contract_violation_line(const void *);
namespace std { namespace contracts { class contract_violation; } }
void handle_contract_violation(const std::contracts::contract_violation &v) {
  printf("kind %d line %u\n", stdc_contract_violation_kind(&v),
         stdc_contract_violation_line(&v));
  fflush(0);
}
struct Label { using assertion_control_object = Label; };
namespace L { inline constexpr Label lbl{}; }
using contract_control namespace L;

int f(int x) _Pre(x > 0) _Post(r : r > 0) {
  _ContractAssert(x > 0);
  return x;
}
struct S {
  int m(int x) const _Pre(x != 1) _Post(r : r != 1) { return x; }
};
template <class T> T t(T x) _Pre(x != 2) _Post(r : r != 2) {
  _ContractAssert(x != 2);
  return x;
}
int lab(int x) _Pre<lbl>(x != 3) { _ContractAssert<(lbl)>(x != 3); return x; }

int main() {
  f(1);
  S().m(0);
  t(0);
  lab(0);
  auto l = [](int x) _Pre(x != 4) { _ContractAssert(x != 4); return x; };
  l(0);
  puts("end");
  f(-1);
  S().m(1);
  t(2);
  lab(3);
  l(4);
  puts("end");
  return 0;
}
