//remark: imported from gcc:intro.compliance.general.p2.3.4.C
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contract_evaluation_semantic=observe
// intro.compliance.general/2.34
// a contract assertion ([basic.contract.eval]) evaluated with a checking
// semantic in a manifestly constant-evaluated context ([expr.const])
// resulting in a contract violation...
// ... shall issue at least one diagnostic message.
// { dg-do compile { target c++23 } }
// { dg-additional-options "-fcontracts -fcontract-evaluation-semantic=observe " }

consteval void foo( auto x ) pre( false ) {}
// { dg-warning {contract predicate is false in constant expression} "" { target *-*-* } .-1 }
int main() {
  foo( 42 );
}
