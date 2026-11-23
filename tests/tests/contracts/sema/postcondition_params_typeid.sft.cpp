//remark:contracts: typeid of a glvalue of polymorphic class type odr-uses a parameter in a postcondition
//type:fn
//match_regex:line 11: error: value parameter "a" used in a postcondition must be declared const
//match_regex:line 15: error: value parameter "a" used in a postcondition must be declared const
//require:BACK_END_IS_CP_GEN_BE 1
#include <typeinfo>
// Its operand is potentially evaluated even where the type of the complete
// object is known (a parameter of class type).
struct B { virtual ~B(); };
struct N { };
bool f(B a) post(typeid(a) == typeid(B)) { return true; }        // Error
bool g(N a) post(typeid(a) == typeid(N)) { return true; }        // OK, unevaluated
bool h(const B a) post(typeid(a) == typeid(B)) { return true; }  // OK
bool i(B &a) post(typeid(a) == typeid(B)) { return true; }       // OK, a reference
template <class T> bool t(T a) post(typeid(a) == typeid(T)) { return true; }  // Error in t<B>
template bool t<B>(B);
template bool t<N>(N);
