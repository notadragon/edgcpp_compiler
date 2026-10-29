//remark: imported from gcc:p3595-dynamic-parse.C
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//source_files: p3595-dynamic-parse-good.json
//options: --c++26 --contracts --contracts_p3400 --contract_configuration_file=p3595-dynamic-parse-good.json
// P3595: output.dynamic parses cleanly (no diagnostics).
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3400" }
// { dg-additional-options "-fcontract-configuration-file=${srcdir}/g++.dg/contracts/cpp26/p3595-dynamic-parse-good.json" }
void f(int x) pre(x > 0) { }
