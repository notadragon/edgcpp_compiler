//remark:contracts: captures by reference, and this, are const in a lambda in a predicate
//type:fn
struct S {
  int m;
  void h() { contract_assert([&] { return ++m > 0; }()); }      // Error
  void h2() { contract_assert([this] { return ++m > 0; }()); }  // Error
  void h3() { contract_assert([*this] { return m > 0; }()); }   // OK
};
void k(int x) { contract_assert([&] { return ++x > 0; }()); }   // Error
void k2(int x) { contract_assert([&x] { return ++x > 0; }()); } // Error
void k3(int x) {
  contract_assert([x]() mutable { return ++x > 0; }());         // OK, a copy
}
void k4(int x) {
  contract_assert([] { int n = 0; return ++n > 0; }());         // OK, its own
}
