//remark:contracts: P4405: --contracts_prove_optimize takes 0, 1, 2, 3 or follow; another value is an error, also in C and without P4405
//type:fc
//options:--contracts_p4405 --contracts_prove_optimize=4:--contracts_p4405 --contracts_prove_optimize=s:--contracts_p4405 --contracts_prove_optimize=01:--contracts_p4405 --contracts_prove_optimize=:--contracts_prove_optimize=Follow
//match_regex:^Command-line error: .*
// Mirror: gcc/testsuite/g++.dg/contracts/cpp26/p4405-prove-optimize-bad.C
// (gnu_gcc).
int x;
