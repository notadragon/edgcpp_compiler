//remark: imported from gcc:contracts-class1.C
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts
// Check whether deferred deduction works correctly when post conditions
// are used in member functions.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts" }

bool check (bool b) { return b; }

bool e;

class S
{
  auto f ()
    post (r: check (r))
    post (r: r)
  { return e; }
};
