//remark: imported from gcc:template-post-pr125011.C
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts
// PR c++/125011
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts" }

template<bool x>
  bool f()
    post(r: r == x)
  {
    return x;
  }

template bool f<true> ();
