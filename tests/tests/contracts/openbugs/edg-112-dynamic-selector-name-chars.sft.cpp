//remark:contracts: EDG-112: a P3595 dynamic selector name that is not made of identifiers is accepted
//require:BACK_END_IS_CP_GEN_BE 1
//options_sep:!
//options:'--contract_configuration=[{"output":{"semantic":"enforce","dynamic":{"linkage":"C++","name":"ns::sel<int>"}}}]';fp!'--contract_configuration=[{"output":{"semantic":"enforce","dynamic":{"linkage":"C++","name":"ns::operator()"}}}]';fp!'--contract_configuration=[{"output":{"semantic":"enforce","dynamic":{"linkage":"C++","name":"sel(int)"}}}]';fp!'--contract_configuration=[{"output":{"semantic":"enforce","dynamic":{"linkage":"C++","name":"ns::1sel"}}}]';fp!'--contract_configuration=[{"output":{"semantic":"enforce","dynamic":{"linkage":"C++","name":"a b"}}}]';fp!'--contract_configuration=[{"output":{"semantic":"enforce","dynamic":{"linkage":"C","name":"sel<int>"}}}]';fp!'--contract_configuration=[{"output":{"semantic":"enforce","dynamic":{"linkage":"C","name":"ns::sel"}}}]';fp!'--contract_configuration=[{"output":{"semantic":"enforce","dynamic":{"linkage":"C","name":"1sel"}}}]';fp!'--contract_configuration=[{"output":{"semantic":"enforce","dynamic":{"linkage":"C","name":""}}}]';fp!'--contract_configuration=[{"output":{"semantic":"enforce","dynamic":{"linkage":"C++","name":"::ok_1::$ok::_ok"}}}]';fp!'--contract_configuration=[{"output":{"semantic":"enforce","dynamic":{"linkage":"C","name":"_ok$2"}}}]';fp
// The recording is the current, wrong behavior: when this test deviates,
// the bug may be fixed.  Correct: cases 1-9 are each "invalid contract
// configuration" ("name" in "dynamic" is not a valid qualified name, or
// identifier for "C" linkage): a template-id, an operator-function-id, a
// parameter list, a component starting with a digit, a space, a qualified
// or empty "C" name; cases 10 and 11 are valid and stay accepted.  GCC
// (GCC-671) and Clang (CLANG-653) reject them at configuration parse.  From
// GCC's p3595-dynamic-bad-name-chars.C (EDG-101).
void f(int x) pre(x > 0) {}
