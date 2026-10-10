//remark: imported from gcc:p3098-ftm.C
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts_p3098
// P3098: the postcondition-captures feature-test macro is defined with the flag.
// (Language feature only; there is no library __cpp_lib_ counterpart.)
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts-p3098" }

#ifndef __cpp_contracts_postcondition_captures
#error "__cpp_contracts_postcondition_captures not defined"
#endif

#if __cpp_contracts_postcondition_captures <= 0
#error "__cpp_contracts_postcondition_captures has wrong value"
#endif
