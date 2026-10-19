//remark:contracts: EDG-65 (deferred): through edg-gxx g++ matches a column-qualified location against the generated code's columns, so the entry that ignores this precondition does not match at run time
//require:BACK_END_IS_CP_GEN_BE 1
//type:rn
//source_files:config.json
//options_all:--contract_evaluation_semantic=quick_enforce --contract_configuration_file=config.json
// The entry names the line and column of the "pre" below, so the violation
// should be ignored and the program print "end" (rp).  cpfe-cp's #line
// directives give g++ the line but not the column, and g++ traps: this
// records the current behavior (DECISIONS.md E17 in notadragon_wg21: the
// deviation is accepted).  The front end matches the column (constant
// evaluation; see contracts/config/constexpr_select).
extern "C" int puts(const char *);

int f(int x)
                    pre(x > 0) { return x; }

int main() {
  f(0);
  puts("end");
  return 0;
}
