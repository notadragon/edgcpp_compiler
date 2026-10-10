//remark:contracts: the noexcept semantics call the violation handler, also with the C-generating back end: a replacement ::handle_contract_violation (reached there through its C-linkage alias) sees noexcept_observe (6) or noexcept_enforce (7); after it returns, noexcept_observe continues and noexcept_enforce aborts; if it throws, the program terminates
//options_all:--contracts_p4298
//options_sep:!
//options:--contract_evaluation_semantic=noexcept_observe -DTHROWS=0;rp!--contract_evaluation_semantic=noexcept_observe -DTHROWS=1;rn!--contract_evaluation_semantic=noexcept_enforce -DTHROWS=0;rn!--contract_evaluation_semantic=noexcept_enforce -DTHROWS=1;rn!'--contract_configuration=[{"match":{"kind":"contract_assert"},"output":{"semantic":"noexcept_observe"}},{"output":{"semantic":"ignore"}}]' -DTHROWS=0;rp
//match_regex:^(semantic [67]|caught|end)$
// (No system headers: also runs under the C-generating back end.)  The
// handler reads the violation through libcontracts' C accessor.
extern "C" int puts(const char *);
extern "C" int printf(const char *, ...);
extern "C" int fflush(void *);
extern "C" int stdc_contract_violation_semantic(const void *);
namespace std { namespace contracts { class contract_violation; } }

void handle_contract_violation(const std::contracts::contract_violation &v) {
  printf("semantic %d\n", stdc_contract_violation_semantic(&v));
  fflush(0);
  if (THROWS) throw 1;
}

int f(int x) pre (x > 0) { return x; }
int g(int x) { contract_assert(x != 2); return x; }

int main() {
  try { f(0); } catch (int) { puts("caught"); }
  try { g(2); } catch (int) { puts("caught"); }
  puts("end");
  return 0;
}
