//remark: imported from gcc:lambda-capture-in-contract-error.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts
//match_regex: ", line 17: (?:catastrophic )?error
//match_regex: ", line 22: (?:catastrophic )?error
//match_regex: ", line 29: (?:catastrophic )?error
// [expr.prim.lambda.capture]/3.3 lets a lambda inside a contract assertion
// have a capture-default or simple-capture, but it does not make an enclosing
// parameter usable without one: with no capture-default and no capture naming
// it, the parameter is not odr-usable ([basic.def.odr]), so referring to it is
// ill-formed.  This must be a diagnostic, not silent acceptance -- Clang
// accepted exactly this and then died in CodeGen.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts" }

void free_fn (int x) pre ([] { return x > 0; } ()) { }
// { dg-error "is not captured" "" { target *-*-* } .-1 }

struct S
{
  void mem (int x) pre ([] { return x > 0; } ()) { }
  // { dg-error "is not captured" "" { target *-*-* } .-1 }
};

void
lambda_own_pre ()
{
  auto l = [] (int x) pre ([] { return x > 0; } ()) { };
  // { dg-error "is not captured" "" { target *-*-* } .-1 }
  (void) l;
}
