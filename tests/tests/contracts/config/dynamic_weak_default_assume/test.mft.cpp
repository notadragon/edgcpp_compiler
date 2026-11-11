//remark:contracts: through edg-gxx a weak default selector whose default semantic is assume takes the ignore path, allowed or adjusted
//require:BACK_END_IS_CP_GEN_BE 1
//type:rp
//source_files:config.json
//options_all:--contract_configuration_file=config.json
//options:-DALLOW=1 --contracts_allow_assume;rp:-DALLOW=0;rp
// GCC's p3595-dynamic-weak-default-assume.C (GCC-690, fixed by M30-GCC per
// DECISIONS.md E66 in notadragon_wg21; this was EDG-123's watch test).  The
// selector and the check are g++'s (cpfe-cp puts the assertion out as
// written): g++ makes a weak default selector returning assume.  With
// assume allowed it takes the ignore path; without, assume is adjusted to
// ignore (D3595R1).  Either way the predicate is not evaluated, nothing is
// reported, and the program prints "start" and "end".
extern "C" int puts(const char *);
int evaluated;
bool chk(int x) { ++evaluated; return x > 0; }
int f(int x) pre(chk(x)) { return x; }
int main() {
  puts("start");
  int r = f(1) + f(-1);
  puts("end");
  return r + evaluated;
}
