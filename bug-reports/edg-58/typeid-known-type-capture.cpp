#include <typeinfo>
struct B { virtual ~B(); };
void f(B x) { auto l = [] { return &typeid(x); }; (void)l; }   // accepted
void g(B &x) { auto l = [] { return &typeid(x); }; (void)l; }  // rejected
