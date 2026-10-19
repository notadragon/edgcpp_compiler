//remark: imported from gcc:contract-assert-warn-attributes.C
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts
// { dg-options "-std=c++26 -fcontracts " }

int foo (int x)
pre [[unlikley]] ( x > 41 )  // { dg-warning "attributes are ignored on function contract specifiers" }
{
  return x + 1;
}
