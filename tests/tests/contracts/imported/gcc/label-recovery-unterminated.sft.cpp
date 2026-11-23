//remark: imported from gcc:label-recovery-unterminated.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --no_contracts_p4451
//match_regex: ", line 25: (?:catastrophic )?error
//match_regex: ", line 28: (?:catastrophic )?error
//match_regex: ", line 31: (?:catastrophic )?error
//match_regex: ", line 35: (?:catastrophic )?error
//match_regex: ", line 40: (?:catastrophic )?error
//match_regex: ", line 41: (?:catastrophic )?error
// Without -fcontracts-p3400 (and -fcontracts-p4451, which C++26 enables and
// which gives such a specifier its own meaning) an assertion-control label is
// diagnosed and its <...> skipped.  When the '>' is missing the skip stops at
// the end of the declaration, so the declarations after it are still parsed
// and diagnosed; it used to run to the end of the file (GCC-665).  The same in
// a class, where the contract is parsed later, and a well-formed label on
// in-class member declarations and definitions reported once each.  With the
// flag the same declaration is contract-label-malformed-recovery.C.
//
// Mirror: clang/test/Contracts/label-recovery-unterminated.cpp in the
// llvm_llvm-project fork (CLANG-648; the class cases are GCC-668).
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fno-contracts-p4451" }

void j (int x) pre<1 x; // { dg-error "assertion-control labels require" }
// { dg-error "expected" "" { target *-*-* } .-1 }
struct A { int a; };
int after = undeclared; // { dg-error "'undeclared' was not declared" }

struct B {
  void k (int x) pre<1 x; // { dg-error "assertion-control labels require" }
  // { dg-error "expected" "" { target *-*-* } .-1 }
  int b;
};
int after2 = undeclared2; // { dg-error "'undeclared2' was not declared" }

struct L { using assertion_control_object = L; };
constexpr L l {};
struct C {
  void m (int x) pre<l> (x > 0); // { dg-error "assertion-control labels require" }
  void n (int x) pre<l> (x > 0) {} // { dg-error "assertion-control labels require" }
};
