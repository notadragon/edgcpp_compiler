//remark:contracts: a precondition's parameters in nested operands (new[] bound, dependent typeid)
//require:BACK_END_IS_CP_GEN_BE 1
//type:rp
//options_all:--contract_evaluation_semantic=enforce
// A parameter may be used anywhere in a precondition's predicate, including
// operands scanned in an expression context of their own.  Neither
// precondition is violated.
#include <typeinfo>
bool f(int x) pre(new int[x] != nullptr) { return true; }
template <class T> bool t(T x) pre(typeid(x) == typeid(int)) { return true; }
int main() { return f(2) && t(1) ? 0 : 1; }
