//remark:contracts: P4405: --contracts_prove_optimize=<0|1|2|3|follow> (our GCC's -fcontracts-prove-optimize=, the proof optimization level) is accepted, the last one given winning; the front end's only proving strategy, folding, is on at every level, so it changes nothing there (the drivers pass it on to g++); without P4405, as for the other P4405 strategies, it warns that it has no effect, whatever its value
//type:fp
//options_sep:!
//options:--contracts_p4405 --contracts_prove_optimize=0!--contracts_p4405 --contracts_prove_optimize=1!--contracts_p4405 --contracts_prove_optimize=2!--contracts_p4405 --contracts_prove_optimize=3!--contracts_p4405 --contracts_prove_optimize=follow!--contracts_p4405 --contracts_prove_optimize=0 --contracts_prove_optimize=follow!--contracts_prove_optimize=1 --contracts_p3850!--contracts --contracts_prove_optimize=0!--contracts_p3850 --no_contracts_p4405 --contracts_prove_optimize=follow!--no_contracts --contracts_prove_optimize=2
//match_regex:^(Command-line (error|warning): .*|)$
// Mirror: gcc/testsuite/g++.dg/contracts/cpp26/p4405-prove-optimize-last.C
// and contract-options-rule-p4405-sub.C (gnu_gcc).
#ifdef __cpp_contracts
bool chk(int x);
int f(int x) pre (chk(x)) post (r: chk(r)) { contract_assert(chk(x)); return x; }
#endif
