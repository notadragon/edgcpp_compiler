//remark: imported from gcc:p4405-analysis-json-runtime.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//source_files: p4405-analysis-json-runtime.json
//options: --c++26 --contracts --contracts_p4405 --contract_configuration_file=p4405-analysis-json-runtime.json
//match_regex: ", line 15: (?:catastrophic )?error
// D4405: the lookup of a runtime evaluation -- here the constant
// evaluation -- skips the entries with "analysis": true, so the first
// entry, which matches every analysis evaluation, does not make the
// precondition ignore: it is enforced in constant evaluation.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p4405" }
// { dg-additional-options "-fcontract-configuration-file=${srcdir}/g++.dg/contracts/cpp26/p4405-analysis-json-runtime.json" }

constexpr int f (int x) pre (x > 0) { return x; }	// { dg-error "contract predicate is false in constant expression" }

constexpr int good = f (1);
constexpr int bad = f (-1);
