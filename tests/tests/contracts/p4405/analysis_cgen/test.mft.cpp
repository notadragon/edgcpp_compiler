//remark:contracts: P4405's analysis evaluation with the C-generating back end: a precondition's is at each call, in the caller (a function, member, constructor, destructor of an automatic object or temporary, template instance, lambda, virtual call), never in the callee nor for an uncalled function; a postcondition's and a contract_assert's in the body, whatever the run-time semantic; nothing is proven, so prove is an error and diagnose a warning; a call in an immediate function, in an "if consteval" branch, in the condition or discarded statement of a constexpr if, or folded to a constant is not analyzed; taking a function's address (&, decay, a reference, a pointer to member function), and destroying an object of static or thread storage duration or a temporary extended to it (through its destructor's address), draws nothing, since no label here allows no run-time semantic (the configuration alone does not determine the calls through the address), but a static array's element destructor calls are analyzed
//require:DO_IL_LOWERING 1
//type:fp
//source_files:analysis_stub.h
//options:--contracts_p3097 --contracts_p4405 --contract_evaluation_semantic=ignore --contract_analysis_semantic=diagnose:--contracts_p3097 --contracts_p4405 --contract_evaluation_semantic=ignore --contract_analysis_semantic=prove;fn
//match_regex:line ([0-9]+): (error|warning): (contract assertion cannot be proven.*|contract predicate is always false.*)
// (No system headers: the C-generating back end cannot find them natively.)
// Under the first options each diagnostic below is a warning; under the
// second an error.  Taking an address draws nothing: no label here allows
// no run-time semantic (see address_cgen).
#include "analysis_stub.h"

constexpr int f(int x)
  pre (x > 0)                    // Not here: at each call.
  post (r: r > 0)                // Here.
{
  contract_assert(x != 3);       // Here.
  return x;
}
int unused(int x) pre (x > 0) { return x; }   // Never called: nothing.

struct S {
  int v;
  S(int x) pre (x > 0) : v(x) {}
  ~S() pre (v > 0) {}
  void m(int x) pre (x > v) {}
  virtual int h(int x) pre (x > 0) { return x; }
};
template <class T> T t(T x) pre (x > 0) { return x; }
template <class T> T never(T x) pre (x > 0) { return x; }

int calls(int y, S &s) {
  int r = f(y);                  // f's precondition.
  r += f(1);                     // A constant call is folded: nothing.
  s.m(y);                        // m's.
  S a(y);                        // The constructor's and the destructor's.
  r += S(y).v;                   // Likewise, for a temporary.
  r += t(y) + (int)t(2L);        // Each instance's.
  r += [](int z) { return f(z); }(y);   // In the lambda's body.
  r += s.h(y);                   // The virtual function's.
  return r;
}

consteval int cf(int x) { return f(x) + 1; }   // Constant: nothing.
constexpr int branches(int y) {
  if consteval { return f(y); } else { return f(y + 1); }   // The else.
}
int use_cf() { return cf(2) + branches(1); }  // branches's precondition-free.

int addresses() {
  int (*p)(int) = &f;            // Address taken: nothing.
  int (*q)(int) = f;             // Likewise, by the decay.
  int (&rf)(int) = f;            // Likewise, a reference bound to it.
  void (S::*pm)(int) = &S::m;    // Likewise, a pointer to member function.
  if (pm == nullptr) return 0;
  return p(1) + q(1) + rf(1);    // Calls through them: nothing.
}

S gs(1);                         // The constructor's (the destructor's
                                 // address, destroyed at exit: nothing).
int statics(int y) {
  static S ls(y);                // Likewise.
  thread_local S tl(y);          // Likewise (destroyed at thread exit).
  static const S &sr = S(y);     // Likewise, for an extended temporary.
  return ls.v + tl.v + sr.v;
}
S arr[2] = {S(1), S(2)};         // Two constructors', the destructor's calls
                                 // for the elements, arr[0]'s cleanup (E160).
int discarded(int y) {
  if constexpr (f(1) > 0) {      // A constant condition: nothing.
    return f(y);                 // The then.
  } else {
    return f(-y);                // Discarded: nothing.
  }
}
