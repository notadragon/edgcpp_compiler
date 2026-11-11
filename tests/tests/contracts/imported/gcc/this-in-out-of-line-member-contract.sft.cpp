//remark: imported from gcc:this-in-out-of-line-member-contract.C
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts
// `this` in a contract on an OUT-OF-LINE member function definition.
//
// `S::call` is a non-static member function however it is written, so
// [expr.prim.this]/2 makes `this` available throughout its declaration, and
// both declarations carry the same contract, so [dcl.contract.func]/6 is
// satisfied.  GCC accepts this, and so does stock g++ 16.2.0.
//
// This test was written because our Clang did not: it rejected the
// out-of-line spelling as "invalid use of 'this' outside of a non-static
// member function" and then tripped an assertion in
// CheckEquivalentContractSequence.  The Clang mirror, since fixed, is
// clang/test/Contracts/this-in-out-of-line-member-contract.cpp in the
// llvm_llvm-project fork.  This file pins that GCC keeps getting it
// right -- a fix on one side must not be mirrored into a regression on
// the other.
//
// Found by porting our grammar tests to Chuanqi Xu's independent Clang
// contracts implementation, whose declarator test covers this shape and ours
// did not.
//
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts" }

struct S {
  int limit;
  int call (int input) const pre (this->limit >= 0);

  // The in-class spelling, which is the one both compilers already accepted.
  int inclass (int input) const pre (this->limit >= 0) { return input; }
};

int S::call (int input) const pre (this->limit >= 0) { return input; }

// A destructor and a const member reached the same way, so the fix is not
// specific to one function kind.
struct T {
  int n;
  ~T () pre (this->n >= 0);
  int get () const post (r : r >= 0);
};

T::~T () pre (this->n >= 0) {}
int T::get () const post (r : r >= 0) { return n; }
