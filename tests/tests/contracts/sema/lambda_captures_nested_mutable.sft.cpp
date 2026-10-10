//remark:contracts: a by-copy capture in a mutable lambda created in a predicate is modifiable
//type:fn
//match_regex:line 18: error: expression must be a modifiable lvalue
// DECISIONS.md E32: in the body of the inner, mutable lambda, "p" names the
// closure member of the by-copy capture, whose type is that of the captured
// entity ([expr.prim.id.unqual] applies the capture rule before the
// constification), and the closure object is created in the predicate.
void param(int p) {
  contract_assert([&] { return [=]() mutable { return ++p; }(); }() > 0);  // OK
}
void local() {
  int p = 0;
  contract_assert([&] { return [=]() mutable { return ++p; }(); }() > 0);  // OK
}
// A variable with static storage duration is not captured: "gp" is const.
int gp = 0;
void global() {
  contract_assert([&] { return [=]() mutable { return ++gp; }(); }() > 0); // Error
}
