//remark:contracts: P4405 virtual functions with the C-generating back end, which proves nothing but folding: for each function a virtual function definition overrides, directly or indirectly (a destructor too, an implicit one included; per subobject when the overridden function's assertions name "this"), its own preconditions and the overridden function's postconditions are override obligations, diagnosed at its definition, naming the overridden function, unless folded to true; an overridden function without contract assertions gives no premise; an overrider this translation unit does not put out is not diagnosed; --no_contracts_prove_override changes nothing; with --no_contracts_prove_fold a predicate is not folded, so a true one is an obligation too (DECISIONS.md E159 (a))
//require:DO_IL_LOWERING 1
//type:fp
//options:--contracts_p3097 --contracts_p4405 --contract_evaluation_semantic=ignore --contract_analysis_semantic=diagnose:--contracts_p3097 --contracts_p4405 --contract_evaluation_semantic=ignore --contract_analysis_semantic=prove;fn:--contracts_p3097 --contracts_p4405 --no_contracts_prove_override --contract_evaluation_semantic=ignore --contract_analysis_semantic=prove;fn:--contracts_p3097 --contracts_p4405 --no_contracts_prove_fold --contract_evaluation_semantic=ignore --contract_analysis_semantic=prove;fn
//match_regex:line ([0-9]+): (error|warning): contract assertion cannot be proven.*
//match_regex:the predicate of the contract assertion at line [0-9]+
// (No system headers: the C-generating back end cannot find them natively.)
// D4405R0, "Virtual Functions"; our GCC's with no override shadows (at -O0,
// or with -fno-contracts-prove-override; DECISIONS.md E150-E157 in
// notadragon_wg21).  Under the first options each obligation is a warning,
// under the others an error.  Nothing here is called, so no precondition
// has an analysis evaluation at a call, and no definition below has a
// postcondition of its own.
bool ok(int);

struct B {
  virtual int f(int x) pre (x > 0) post (r: r > 0);
};
struct D : B {
  int f(int x) override pre (x >= 0);
};
int D::f(int x) { return x + 1; }       // D::f's pre and B::f's post.

struct B2 {                             // No contract assertions.
  virtual int g(int x);
  virtual int h(int x);
};
struct D2 : B2 {
  int g(int x) override pre (x > 100);
  int h(int x) override pre (true) pre (sizeof(int) > 1);
};
int D2::g(int x) { return x; }          // Its pre, from nothing.
int D2::h(int x) { return x; }          // Folded to true, if folding: nothing.

struct B3 { virtual int f(int x) pre (x > 0) post (r: r > 0); };
struct C3 : B3 { int f(int x) override pre (x > 1) post (r: r > 1); };
struct D3 : C3 { int f(int x) override; };
int D3::f(int x) { return x + 2; }      // C3::f's post, then B3::f's.

struct V { virtual int f(int x) post (r: r != 0); };
struct L : virtual V { };
struct R : virtual V { };
struct J : L, R { int f(int x) override; };
int J::f(int x) { return 1; }           // One virtual base: once.

struct N { int m; virtual int f(int x) post (r: r > m); };
struct N1 : N { };
struct N2 : N { };
struct NJ : N1, N2 { int f(int x) override; };
int NJ::f(int x) { return x; }          // Names "this": per subobject, twice.

struct M { virtual int f(int x) post (r: r > 0); };
struct M1 : M { };
struct M2 : M { };
struct MJ : M1, M2 { int f(int x) override; };
int MJ::f(int x) { return x; }          // Does not: once.

struct BD { virtual ~BD() post (ok(0)); };
struct DD : BD { virtual void key(); };  // Implicit ~DD, put out with the
void DD::key() { }                      // vtable: at the class.
struct UD : BD { ~UD() pre (ok(1)); };
UD::~UD() { }                           // Its pre and BD's post.

template <class T> struct TB {
  virtual T f(T x) pre (x > 0) post (r: r > 0);
};
template <class T> struct TD : TB<T> {
  T f(T x) override { return x; }
};
template struct TD<int>;                // TB<int>::f's post, per instance.

struct KB { virtual int f(int x) post (r: r > 0); };
struct KE : KB {                        // Key function elsewhere: the
  int f(int x) override { return x; }   // inline f is not put out here.
  virtual void key();
};
struct KH : KB {                        // Key function here: it is.
  int f(int x) override { return x; }
  virtual void key();
};
void KH::key() { }

struct P : B { int f(int x) override = 0; };   // Pure: nothing.
