//remark: imported from gcc:pr113968.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts
//match_regex: ", line 18: (?:catastrophic )?error
//match_regex: ", line 19: (?:catastrophic )?error
//match_regex: ", line 24: (?:catastrophic )?error
//match_regex: ", line 25: (?:catastrophic )?error
//match_regex: ", line 27: (?:catastrophic )?error
//match_regex: ", line 28: (?:catastrophic )?error
// check that an invalid contract condition doesn't cause an ICE
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts" }
struct A {
  A(A &);
};
struct S{
  void f(A a) pre(a) // { dg-error "could not convert" }
              pre(a.b) // { dg-error "has no member" }
  {

  }
};
void f(A a) pre(a) // { dg-error "could not convert" }
	    pre(a.b) // { dg-error "has no member" }
	    {
  contract_assert(a); // { dg-error "could not convert" }
  contract_assert(a.b); // { dg-error "has no member" }
}

int main()
{
}
