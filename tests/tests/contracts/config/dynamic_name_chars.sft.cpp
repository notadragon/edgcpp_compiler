//remark:contracts: a P3595 dynamic selector name must be identifiers joined by "::" ("C++") or one identifier ("C"), as in GCC and Clang
//require:BACK_END_IS_CP_GEN_BE 1
//options_sep:!
//options:'--contract_configuration=[{"output":{"semantic":"enforce","dynamic":{"linkage":"C++","name":"ns::sel<int>"}}}]';fc!'--contract_configuration=[{"output":{"semantic":"enforce","dynamic":{"linkage":"C++","name":"ns::operator()"}}}]';fc!'--contract_configuration=[{"output":{"semantic":"enforce","dynamic":{"linkage":"C++","name":"sel(int)"}}}]';fc!'--contract_configuration=[{"output":{"semantic":"enforce","dynamic":{"linkage":"C++","name":"ns::1sel"}}}]';fc!'--contract_configuration=[{"output":{"semantic":"enforce","dynamic":{"linkage":"C++","name":"a b"}}}]';fc!'--contract_configuration=[{"output":{"semantic":"enforce","dynamic":{"linkage":"C","name":"sel<int>"}}}]';fc!'--contract_configuration=[{"output":{"semantic":"enforce","dynamic":{"linkage":"C","name":"ns::sel"}}}]';fc!'--contract_configuration=[{"output":{"semantic":"enforce","dynamic":{"linkage":"C","name":"1sel"}}}]';fc!'--contract_configuration=[{"output":{"semantic":"enforce","dynamic":{"linkage":"C","name":""}}}]';fc!'--contract_configuration=[{"output":{"semantic":"enforce","dynamic":{"linkage":"C++","name":"::ok_1::$ok::_ok"}}}]';fp!'--contract_configuration=[{"output":{"semantic":"enforce","dynamic":{"linkage":"C","name":"_ok$2"}}}]';fp
// Cases 1-9 are each "invalid contract configuration" ("name" in "dynamic"
// is not a valid qualified name, or identifier for "C" linkage): a
// template-id, an operator-function-id, a parameter list, a component
// starting with a digit, a space, a qualified or empty "C" name.  Cases 10
// and 11 are valid ("$" is an identifier character).  From GCC's
// p3595-dynamic-bad-name-chars.C (EDG-101; GCC-671, CLANG-653).
void f(int x) pre(x > 0) {}
