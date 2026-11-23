//remark: imported from gcc:p3290-api-no-flag.C
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26
// P3290: Gate macro not defined without the flag.
// { dg-do compile { target c++26 } }

#ifdef __gcc_contracts_p3290
#error "__gcc_contracts_p3290 should not be defined without -fcontracts-p3290"
#endif
