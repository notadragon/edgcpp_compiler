//remark:contracts: no violation is reported from an assumption's operand in constant evaluation
//type:fp
//options_all:--contract_evaluation_semantic=quick_enforce
// Also runs under the C-generating back end.  The operand of an assumption
// is not evaluated as far as the program is concerned, so a contract
// assertion met while the front end tries it is not violated.
constexpr bool always_false(int *p) { contract_assert(*p > 100); return true; }
constexpr int assume_operand() {
  int i = 0;
  [[assume(always_false(&i))]];
  return i;
}
static_assert(assume_operand() == 0);
