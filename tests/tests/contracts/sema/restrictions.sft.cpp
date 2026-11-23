//remark:contracts: where function contract specifiers and result names are not allowed
//type:fn
struct V { virtual void f(int x) pre(x > 0); };    // Error, virtual
struct B { virtual void g(int x); };
struct D : B { void g(int x) pre(x > 0); };        // Error, overrides: virtual
struct E { E() pre(true) = default; };             // Error, defaulted
struct E2 { E2() pre(true); };
E2::E2() pre(true) = default;                      // OK, not on the first
void del(int x) pre(x > 0) = delete;               // Error, deleted
void vr() post(r: true);                           // Error, void result
struct C {
  C() post(r: true);                               // Error, constructor
  ~C() post(r: true);                              // Error, destructor
};
int sh(const int r) post(r: r > 0);                // Error, shadows
int sh2(int _) post(_: _ > 0);                     // OK, placeholder
auto ded(int x) post(r: r > 0);                    // Error, not a definition
auto ded2(const int x) post(x > 0) { return x; }   // OK, no result name
using FP = void (*)(int) pre(true);                // Error, type-id
typedef void FT(int) pre(true);                    // Error, typedef
void (*fp)(int) pre(true);                         // Error, not a function
int a, f() pre(true), b pre(true);                 // Error, b
