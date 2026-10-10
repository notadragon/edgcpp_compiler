//remark: imported from gcc:ce-observe-rt-enforce.C
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//source_files: ce-observe-rt-enforce.json
//options: --c++26 --contracts --contract_configuration_file=ce-observe-rt-enforce.json
// CE semantic: observe (warning only).  Runtime semantic: enforce.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts" }
// { dg-additional-options "-fcontract-configuration-file=${srcdir}/g++.dg/contracts/cpp26/ce-observe-rt-enforce.json" }

constexpr int f(int x) pre(x >= 0) { return x; } // { dg-warning "contract predicate is false in constant expression" }
constexpr int y = f(-1);
