//remark:contracts: errors in the contract assertions of member functions
//type:fn
//cases:3
// The predicates of a member function are scanned when its class is
// complete, so they can name members declared after them.
#if TEST_NUMBER == 1
struct S {
  int f(int x) pre(y > z);      // Error, z undeclared (y is a member)
  int y;
};
#elif TEST_NUMBER == 2
struct S {
  void g() post(r: r > 0);      // Error, result name for a void function
};
#elif TEST_NUMBER == 3
struct S {
  int h(int x) post(r: r > 0) post(r: r < 10) post(x > r);  // Error, third r
};
#endif
