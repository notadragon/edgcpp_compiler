//remark:contracts: P3097 in constant evaluation: a virtual call checks the interface's and the implementation's contract assertions in order
//type:fp
//options_all:--contracts_p3097 --contract_evaluation_semantic=quick_enforce '--contract_configuration=[{"match":{"constexpr":true},"output":{"semantic":"observe"}}]'
//match_regex:line 12: warning: contract predicate is false in constant expression
//match_regex:line 17: warning: contract predicate is false in constant expression
// In constant evaluation violations are observed (warnings), so the recording
// shows which assertions are checked, in order: a virtual call checks the
// interface (the statically chosen function) around the implementation (the
// final overrider); a qualified call and a call through a pointer to member
// check only the function called.
struct B {
  constexpr virtual int f(int x) const pre(x > 10) post(r: r > 100) {
    return x;
  }
};
struct D : B {
  constexpr int f(int x) const override pre(x > 20) post(r: r > 200) {
    return x;
  }
};
constexpr int call(const B &b, int x) { return b.f(x); }
constexpr D d{};
constexpr int v1 = call(d, 5);                  // B pre, D pre, D post, B post
constexpr int v2 = d.B::f(5);                   // B pre, B post
constexpr int v3 = (static_cast<const B &>(d).*(&B::f))(5);  // D pre, D post
constexpr int v4 = call(B{}, 50);               // B post only (not virtual)

// "this" in the interface's assertions is the interface's subobject, in the
// implementation's the overrider's; a covariant result is seen by each
// postcondition as its own type.  No violation here.
struct X { int pad = 9; };
struct R1 { int v = 1; };
struct R2 : X, R1 { int w = 2; };
constexpr R2 r2{};
struct I {
  int k = 7;
  constexpr virtual const R1 *g() const pre(k == 7) post(r: r->v == 1) {
    return &r2;
  }
};
struct M : X, I {
  int m = 3;
  constexpr const R2 *g() const override pre(m == 3) post(r: r->w == 2) {
    return &r2;
  }
};
constexpr M mm{};
constexpr int v5 = static_cast<const I &>(mm).g()->v;

int use() { return v1 + v2 + v3 + v4 + v5; }
