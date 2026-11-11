//remark:contracts: P4405 with the C-generating back end: report once per outer evaluation (DECISIONS.md E169 (9), (11), (15) in notadragon_wg21): a nested diagnose precondition is warned once for the analysis evaluation of an assertion and the check of that assertion in the same function (the evaluation at the same call in the check is not warned again), but once for each outer evaluation otherwise: f's precondition analyzed at each of three calls warns three times, once in each of two template instances, and a precondition checked in its function and analyzed at a call warns twice at the call in its predicate
//require:DO_IL_LOWERING 1
//type:fn
//options_sep:!
//options:--contracts_p4405 --contract_analysis_semantic=prove '--contract_configuration=[{"match":{"analysis":true,"namespace":"dd"},"output":{"semantic":"diagnose"}},{"match":{"kind":"pre","namespace":"pinned"},"output":{"semantic":"quick_enforce"}},{"match":{"kind":"pre"},"output":{"semantic":"ignore"}},{"output":{"semantic":"quick_enforce"}}]'
//match_regex:line ([0-9]+): (error|warning): (contract assertion cannot be proven|contract predicate is always false)
// (No system headers: the C-generating back end cannot find them natively.)
// GCC's p4405-nested-report-once-O0.C (imported in imported_p4405/gcc,
// where g++ decides), with quick_enforce for its enforce (the C-generating
// back end's), plus the instances t<int>/t<long> and a call in a checked
// predicate whose precondition has a nested one (once).  Nothing proves an
// outer assertion here (EDG-134), so each is an error.
// Mirrors: GCC's p4405-nested-report-once{,-O0}.C, Clang's
// p4405-nested-report-once.cpp.

namespace dd {
int g5(int x) pre(x != 0);
int g5(int) { return 1; }
}

// Checked and analyzed in the same place: once.
int single(int y) {
  contract_assert(dd::g5(y) == 1);
  return y;
}

// Two assertions, each its own pair: once each.
int two(int y) {
  contract_assert(dd::g5(y) == 1);
  contract_assert(dd::g5(y + 1) == 1);
  return y;
}

// A postcondition, analyzed and checked in p: once.
int p(int y) post(r: dd::g5(r) == 1) { return y; }

// f's precondition is only analyzed (preconditions are ignore at run time
// here), at each of three calls: three outers, three warnings at g5's call.
int f(int y) pre(dd::g5(y) == 1);
int f(int y) { return y; }
int c1(int y) { return f(y); }
int c2(int y) { return f(y) + f(y + 1); }

// Two instances, two outers: twice.
template <typename T> int t(T y) { return f(y); }
template int t<int>(int);
template int t<long>(long);

// A call in a checked assertion: f's precondition analyzed at the call in
// the check belongs to the assertion's group, so g5's nested evaluation
// under it is the one under the assertion's analysis: once.
int through(int y) {
  contract_assert(f(y) == 1);
  return y;
}

// f2's precondition is checked in f2 and analyzed at c3's call: two groups,
// two warnings at g5's call (E169 (15)).
namespace pinned {
int f2(int y) pre(dd::g5(y) == 1);
int f2(int y) { return y; }
}
int c3(int y) { return pinned::f2(y); }
