//remark: imported from gcc:p4451-constinit.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts_p4451 --contract_evaluation_semantic=ignore
//match_regex: ", line 16: (?:catastrophic )?error
// P4451: whether a variable is constant-initialized is decided with every
// contract assertion using the ignore semantic ([expr.const.init]), so the
// initializer below is a constant initializer; its evaluation then uses a
// terminating semantic, and the false predicate makes it fail.  Under -fcontracts-p4451 alone.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts-p4451 -fcontract-evaluation-semantic=ignore" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <contracts>

constexpr int l (int x) pre<terminating> (x != 1) { return x; }	// { dg-error "contract predicate is false" }
const int i1 = l (1);
