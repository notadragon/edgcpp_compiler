//remark:contracts: P4405: prove and diagnose do not evaluate the predicate, under both back ends; each diagnose evaluation is warned about once, by the front end with the C-generating back end and by g++ alone through edg_gxx.sh; without --contracts_p4405 prove is ignore (its warning, which g++ gives too, is tested by p4405/option; without a location g++'s names a temporary file, hence --no_warnings)
//type:rp
//options_sep:!
//options:--contracts_p4405 --contract_evaluation_semantic=diagnose!--contracts_p4405 '--contract_configuration=[{"output":{"semantic":"diagnose"}}]'!--contract_evaluation_semantic=prove --no_warnings
//match_regex:^(f 0|evaluated 0|end)$
// (No system headers: also runs under the C-generating back end.)
extern "C" int printf(const char *, ...);
int evaluated = 0;
bool check(bool b) { ++evaluated; return b; }
int f(int x) pre (check(x > 0)) post (r: check(r > 0)) {
  contract_assert(check(x > 1));
  return x;
}
int main() {
  printf("f %d\n", f(0));
  printf("evaluated %d\n", evaluated);
  printf("end\n");
  return 0;
}
