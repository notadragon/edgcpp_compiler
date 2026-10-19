//remark: imported from gcc:p3595-dynamic-selector-wrong-type-quiet.C
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//source_files: p3595-dynamic-selector-wrong-type.json
//options: --c++26 --contracts --contract_configuration_file=p3595-dynamic-selector-wrong-type.json
// GCC-672: p3595-dynamic-selector-wrong-type.C under
// -Wno-contract-configuration, which silences every warning there.  (The
// included file's dg- directives are not read.)
//
// Mirror: the -Wno-contract-configuration RUN line of
// clang/test/Contracts/p3595-dynamic-selector-wrong-type.cpp in the
// llvm_llvm-project fork (CLANG-655).
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -Wno-contract-configuration" }
// { dg-additional-options "-fcontract-configuration-file=${srcdir}/g++.dg/contracts/cpp26/p3595-dynamic-selector-wrong-type.json" }

#include "p3595-dynamic-selector-wrong-type.C"
