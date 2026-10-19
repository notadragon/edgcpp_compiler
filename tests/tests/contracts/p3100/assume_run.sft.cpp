//remark:contracts: P3100's assume semantic does not evaluate the predicate of a contract assertion, under both back ends, from the command line or a configuration; without --contracts_allow_assume it is ignore
//type:rp
//options_sep:!
//options:--contract_evaluation_semantic=assume!--contracts_allow_assume --contract_evaluation_semantic=assume!--contracts_allow_assume '--contract_configuration=[{"output":{"semantic":"assume"}}]'!'--contract_configuration=[{"output":{"semantic":"assume"}}]'
//match_regex:^(f 0|evaluated 0|end)$
// (No system headers: also runs under the C-generating back end, which
// allows assume as it does ignore.)
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
