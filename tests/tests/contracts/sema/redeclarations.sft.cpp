//remark:contracts: function contract specifiers on redeclarations
//type:fn
// A redeclaration repeats the contracts of the first declaration exactly (as
// written: a parameter is identified by position, a result name by its
// postcondition), or omits them.
int f1(int x) pre(x > 0);
int f1(int x) pre(x > 0) { return x; }        // OK
int f2(int x);
int f2(int x) pre(x > 0);                     // Error, adds contracts
int f3(int x) pre(x > 0);
int f3(int x) pre(x > 0) pre(x < 9);          // Error, number
int f4(int x) pre(x > 0);
int f4(const int x) post(x > 0);              // Error, kind
int f5(int x) pre(x > 0);
int f5(int x) pre(x >= 1);                    // Error, condition
int f6(int y) pre(y > 0);
int f6(int z) pre(z > 0) { return z; }        // OK, parameter renamed
int f7(int x) post(r: r > 0);
int f7(int x) post(r > 0);                    // Errors, result name
int f8(const int x) post(r: r > x);
int f8(const int y) post(s: s > y) { return y; }  // OK, renamed
int f9(int x) pre(x > 0);
int f9(int x) { return x; }                   // OK, omitted
void f10();
void f10() pre(undeclared) { }                // Error, only the predicate

// Members: the out-of-class definition is checked against the class's
// declaration (whose predicate is scanned when the class is complete).
struct S {
  int m;
  int f(int x) pre(x > m);
  int g(int x) pre(x > 0);
  int h(int x);
};
int S::f(int y) pre(y > m) { return y; }      // OK
int S::g(int x) pre(m > 0) { return x; }      // Error, condition
int S::h(int x) pre(x > 0) { return x; }      // Error, adds contracts

// Friends, including a friend declared in a class definition after the
// function, whose predicate is matched when the class is complete.
struct F { friend int ff(int x) pre(x > 0); };
int ff(int x) pre(x > 1);                     // Error, condition
int gg(int x) pre(x > 0);
struct G { friend int gg(int x) pre(x > 0); };    // OK
int hh(int x) pre(x > 0);
struct H { friend int hh(int x) pre(x > 2); };    // Error, condition

// Templates.
template <class T> T t1(T x) pre(x > 0);
template <class T> T t1(T x) pre(x > 0) { return x; }          // OK
template <class T> T t2(T x) pre(x > 0);
template <class T> T t2(T x) pre(x > 1) { return x; }          // Error
template <class T> T t3(T x) pre(x > 0) { return x; }
template <> int t3<int>(int x) pre(x > 5) { return x; }        // OK, not matched
template <class T> struct CT { int f(int x) pre(x > 0); };
template <class T> int CT<T>::f(int x) pre(x > 0) { return x; }  // OK
template <class T> struct CT2 { int f(int x) pre(x > 0); };
template <class T> int CT2<T>::f(int x) pre(x > 1) { return x; } // Error
int use() { return t1(1) + t3(1) + CT<int>().f(1); }
