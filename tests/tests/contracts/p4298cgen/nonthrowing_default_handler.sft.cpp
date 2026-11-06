//remark:contracts: without a replacement handler the noexcept semantics call the default violation handler (libstdc++exp's, which eccp links when a check calls libcontracts), and noexcept_observe continues after it
//options_all:--contracts_p4298 --contract_evaluation_semantic=noexcept_observe
//type:rp
//match_regex:^end$
// (No system headers: also runs under the C-generating back end.)
extern "C" int puts(const char *);
int f(int x) pre (x > 0) { return x; }
int main() {
  f(0);
  puts("end");
  return 0;
}
