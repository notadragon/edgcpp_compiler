//remark: imported from gcc:p4301-report-ftm-undefined.C
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts
// P4301: without -fcontracts-p4301 neither the compiler nor the library
// feature-test macro for contract_violation::report() should be defined,
// even though contracts are otherwise enabled.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#ifdef __cpp_contracts_report
#error "__cpp_contracts_report unexpectedly defined"
#endif

#include <contracts>

#ifdef __cpp_lib_contracts_report
#error "__cpp_lib_contracts_report unexpectedly defined"
#endif
