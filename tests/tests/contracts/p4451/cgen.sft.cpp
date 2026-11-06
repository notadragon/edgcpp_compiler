//remark:contracts: with the C-generating back end a terminating contract assertion (P4451) is quick_enforce whatever the configured semantic, as P3400's labels::terminating is (the only terminating semantic it checks); through g++ it is enforced
//options_all:--contracts_p4451
//options:--contract_evaluation_semantic=ignore -DVIOLATE=0;rp:--contract_evaluation_semantic=ignore -DVIOLATE=1;rn:--contract_evaluation_semantic=ignore -DVIOLATE=2;rn:--contract_evaluation_semantic=quick_enforce -DVIOLATE=1;rn
// No system headers: the declaration the specifier needs is the test's own.
extern "C" int puts(const char *);
namespace std::contracts::labels {
  inline constexpr struct terminating_t {} terminating{};
}

int f(int x) pre<terminating>(x > 0) { return x; }
int g(int x) { contract_assert<terminating>(x != 3); return x; }

int main() {
  if (VIOLATE == 0) { f(1); g(1); }
  if (VIOLATE == 1) f(0);
  if (VIOLATE == 2) g(3);
  puts("end");
  return 0;
}
