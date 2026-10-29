//remark:contracts: P4301: contract_violation::report() is null for a violation the front end's checks (or g++'s) report, under both back ends
//options_all:--contracts_p4301 --contracts_p4298 --contract_evaluation_semantic=noexcept_observe
//type:rp
//match_regex:^(report (null|nonnull)|end)$
// (No system headers: also runs under the C-generating back end.)  The
// member is declared by hand, as libstdc++'s <contracts> declares it with
// --contracts_p4301 (defined in libstdc++exp).
extern "C" int puts(const char *);
namespace std { namespace contracts {
class contract_violation {
public:
  const char *report() const noexcept;
};
} }
void handle_contract_violation(const std::contracts::contract_violation &v) {
  puts(v.report() == nullptr ? "report null" : "report nonnull");
}
int f(int x) pre (x > 0) { contract_assert(x > 1); return x; }
int main() {
  f(0);
  puts("end");
  return 0;
}
