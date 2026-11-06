//remark:contracts: an id-expression naming a variable, and this, are const in a predicate
//type:fn
int g;
struct S {
  int m;
  mutable int mm;
  void f(int x) pre(++m > 0);                    // Error, *this is const
  void h() pre(++g);                             // Error, a global too
  void k() pre(++mm > 0);                        // OK, mutable member
  void b() { contract_assert(++m); }             // Error
  void t() pre(this->m++ > 0);                   // Error
};
void a(int x) pre(++x > 0);                      // Error, a parameter
void b(int &x) pre(++x > 0);                     // Error, through a reference
int c(int x) post(r: ++r > 0);                   // Error, the result name
void d() { int l = 0; contract_assert(++l); }    // Error, a local variable
void e(int x) pre(sizeof(++x) > 0);              // Error, also unevaluated
struct P { int a, b; };
void sb() { auto [u, v] = P{1, 2}; contract_assert(++u); }  // Error
void ok() { int l = 0; contract_assert([] { int n = 0; return ++n; }() > l); }  // OK
