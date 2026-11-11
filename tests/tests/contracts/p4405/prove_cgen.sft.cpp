//remark:contracts: P4405: with the C-generating back end the front end diagnoses each analysis evaluation with prove (an error) or diagnose (a warning): a postcondition's and a contract_assert's where the check would be generated, once per assertion of each definition (per instance of a template, not in an uninstantiated one, not again in an interface wrapper, P3097), and a precondition's at each call (an uncalled function's never); a prove or diagnose from the command line or a configuration moves to the analysis evaluation
//require:DO_IL_LOWERING 1
//type:fn
//options_sep:!
//options:--contracts_p4405 --contracts_p3097 --contract_evaluation_semantic=prove;fn!--contracts_p4405 --contracts_p3097 --contract_evaluation_semantic=diagnose;fp!--contracts_p4405 --contracts_p3097 '--contract_configuration=[{"match":{"kind":"pre"},"output":{"semantic":"prove"}},{"output":{"semantic":"diagnose"}}]';fn
//match_regex:line ([0-9]+): (error|warning): contract assertion cannot be proven
// (No system headers: the C-generating back end cannot find them natively.)
// Under the first options an error at each line marked, under the second a
// warning; under the third an error for each precondition (at its calls)
// and a warning for the rest.  An inline function that is never called,
// and an uninstantiated template, are not diagnosed; the body of a
// template is diagnosed per instance (t's instances' have no assertions in
// the body, so only their calls are diagnosed, twice).

int f(int x)
  pre (x > 0)
  post (r: r > 0)                // Diagnosed.
{
  contract_assert(x != 3);       // Diagnosed.
  return x;
}
int use_f(int y) { return f(y); }   // f's precondition.

template <class T>
inline T t(T x)
  pre (x > 0)
{ return x; }
template <class T>
inline T never(T x)
  pre (x > 0)
{ return x; }
int use_t() { return t(1) + (int)t(2L); }   // Each instance's, once.

struct V { virtual ~V() {} };
struct S : virtual V {
  int v;
  S(int x) pre (x > 0) : v(x) {}
  ~S() { contract_assert(v > 0); }  // Diagnosed once.
};
S s(1);                          // The constructor's precondition.

struct B {
  virtual int h(int x) pre (x > 0) { return x; }
};
int call(B &b) { return b.h(1); }   // h's precondition, not again in
                                    // the interface wrapper.

inline int unused_inline(int x) pre (x > 0) { return x; }

constexpr int ce(int x) pre (x > 0) { return x; }
static_assert(ce(1) == 1);       // Constant evaluation: never analyzed.
