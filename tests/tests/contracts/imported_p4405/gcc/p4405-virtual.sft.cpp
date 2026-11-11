//remark: imported from gcc:p4405-virtual.C
//type: cn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3097 --contracts_p4405 --contract_evaluation_semantic=prove
//match_regex: Test_name.c:26:[0-9]+: error: 
//match_regex: Test_name.c:33:[0-9]+: error: 
// EDG: adapted -- the front end gives no analysis diagnostic through edg_gxx.sh:
// g++ makes the analysis evaluations (and applies the address-of rule), so
// the test compiles (type cn) and matches g++'s diagnostics.
// P4405: the contracts of a virtual function (P3097) under prove.  A
// postcondition is diagnosed in the function's own body, and not again in
// the wrapper a virtual call goes through, which evaluates it again at run
// time with the callee's semantics.  A precondition is analyzed at each
// call (D4405) -- a virtual call too, for the function it names -- and not
// in the body or the wrapper.  (the number expected) makes a
// repeated error fail the test.
//
// Mirror: clang/test/Contracts/p4405-virtual.cpp in the llvm_llvm-project
// fork.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3097 -fcontracts-p4405 -fcontract-evaluation-semantic=prove" }

struct B {
  virtual int h (int x)
    pre (x > 0)			// { dg-message "the predicate of the contract assertion" }
    post (r: r > 1)		// { dg-error "contract assertion cannot be proven" }
  { return x; }
};

int call (B &b, int y)
{
  int r = b.h (1);		// { dg-bogus "cannot be proven" }
  return r + b.h (y);		// { dg-error "contract assertion cannot be proven" }
}
