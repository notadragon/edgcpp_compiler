//remark:contracts: under a noexcept semantic eccp links libstdc++ after the contracts runtime (libstdc++exp needs it), so the link also works statically
//require:DO_IL_LOWERING 1
//type:rp
//options_all:--contracts_p4298 --contract_evaluation_semantic=noexcept_observe --driver_debug
//filter:grep -oE -- '-lstdc\+\+exp -lcontracts( -lstdc\+\+)?|^end$'
//match_regex:^-lstdc\+\+exp -lcontracts -lstdc\+\+$
//match_regex:^end$
// The suite links dynamically, where a shared libstdc++ earlier on the line
// would do, so the filter keeps the end of eccp's link command
// (--driver_debug echoes it) and the program's last line.
extern "C" int puts(const char *);
int f(int x) pre (x > 0) { return x; }
int main() {
  f(0);
  puts("end");
  return 0;
}
