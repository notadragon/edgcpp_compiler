//remark:contracts: the contracts option rule (notadragon_wg21 DECISIONS.md E175), as our GCC's contract-options-additive-link-*: a later --no_ form of one paper option leaves contracts on, and g++ links the contracts runtime, while another paper option is on (C++23, so that only the options turn contracts on)
//require:BACK_END_IS_CP_GEN_BE 1
//type:rp
//options:--c++23 --contracts_p3400 --no_contracts_p3850:--c++23 --contracts_p3850 --no_contracts_p3400:--c++23 --contracts_p4451 --no_contracts_p3400:--c++23 --contracts_p3098 --no_contracts_p4405
//
// Mirror: gcc/testsuite/g++.dg/contracts/cpp26/contract-options-additive-link-*.C
int f(int x) pre (x > 0) { return x; }
int main() { return f(1) - 1; }
