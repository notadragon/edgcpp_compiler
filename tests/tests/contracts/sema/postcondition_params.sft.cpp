//remark:contracts: parameters odr-used in postconditions must be const
//type:fn
void e(int x) post(x > 0);                       // Error
void e2(const int x) post(x > 0);                // OK
void e3(int &x) post(x > 0);                     // OK, a reference
void e4(int x) post(sizeof(x) > 0);              // OK, unevaluated
int e5(const int x) post(r: r > x);
int e5(int x) { return x; }                      // Error, on the definition
int e6(int x) post(r: r > x) { return x; }       // Error (once)
auto l = [](int y) post(y > 0) { return y; };    // Error, a lambda's too
template <class T> T t(T x) post(x > 0) { return x; }
int use_t = t(1);                                // Error, in the instance
template <class T> T t2(T x) post(x > 0) { return x; }
int use_t2 = t2<const int>(1);                   // OK
template <class... T> void pk(T... x) post(((x > 0) && ...)) {}  // Error
void use_pk() { pk(1, 2); }                      // (in this instance)
