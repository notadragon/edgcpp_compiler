//remark: imported from gcc:p3097-ftm.C
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3097
// P3097: Feature-test macro -- bumps __cpp_contracts above 202502L.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3097" }

#if __cpp_contracts <= 202502L
#error "__cpp_contracts not bumped above 202502L with -fcontracts-p3097"
#endif
