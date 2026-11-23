//remark:contracts: --contract_violation_information takes full or none; another value is an error
//type:fc
//options_all:--contract_violation_information=some
//match_regex:unrecognized contract violation information \(must be full or none\): some
// Mirror: clang/test/Contracts/contract-violation-information-option.c
// (llvm_llvm-project), its invalid value.
int x;
