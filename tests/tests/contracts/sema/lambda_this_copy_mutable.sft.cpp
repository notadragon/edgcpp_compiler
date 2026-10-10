//remark:contracts: a mutable lambda in a contract predicate may modify its own copy of *this (EDG-80)
//type:fp
//options_all:--contract_evaluation_semantic=quick_enforce
// The copy is a member of the closure object, not the object "this" points
// to in the predicate, which stays const.
struct S {
  int m;
  void f() pre([*this]() mutable { return ++m > 0; }());
  void g() {
    contract_assert([*this]() mutable { return ++m > 0 && ++this->m > 0; }());
  }
  void h() pre([=, *this]() mutable { return ++m > 0; }());
  void k() pre([*this]() mutable { return [this] { return ++m > 0; }(); }());
};
