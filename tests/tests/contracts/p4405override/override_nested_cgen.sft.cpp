//remark:contracts: P4405 virtual functions with the C-generating back end: an override obligation has the nested evaluations of the calls in its predicate (DECISIONS.md E169 (8), (16) in notadragon_wg21), decided by folding with nothing known of the parameters: a nested prove one not proven is noted on the obligation's diagnostic ("precondition of a call in the predicate cannot be proven" / "... is always false"), and makes even a predicate folded to true an obligation; a nested diagnose one not proven is warned at its call in B::f's or D::f's predicate with the obligation's text naming B::f, with a note at its predicate, also when the obligation itself is folded to true, and not at all when D::f is not put out (E150); with --no_contracts_prove_fold nothing is folded
//require:DO_IL_LOWERING 1
//type:fn
//options_sep:!
//options:--contracts_p3097 --contracts_p4405 --contract_evaluation_semantic=ignore --contract_analysis_semantic=prove '--contract_configuration=[{"match":{"analysis":true,"namespace":"dd"},"output":{"semantic":"diagnose"}}]'!--contracts_p3097 --contracts_p4405 --no_contracts_prove_override --contract_evaluation_semantic=ignore --contract_analysis_semantic=prove '--contract_configuration=[{"match":{"analysis":true,"namespace":"dd"},"output":{"semantic":"diagnose"}}]'!--contracts_p3097 --contracts_p4405 --no_contracts_prove_fold --contract_evaluation_semantic=ignore --contract_analysis_semantic=prove '--contract_configuration=[{"match":{"analysis":true,"namespace":"dd"},"output":{"semantic":"diagnose"}}]'
//match_regex:line ([0-9]+): (error|warning): (contract assertion cannot be proven.*)
//match_regex:(the predicate of the contract assertion at line [0-9]+)
//match_regex:(precondition of a call in the predicate (cannot be proven|is always false) at line [0-9]+)
// (No system headers: the C-generating back end cannot find them natively.)
// GCC's p4405-override-nested-O0.C (imported in imported_p4405override/gcc,
// where g++ decides: there -O0, -O2 and -fno-contracts-prove-override
// differ; here, with no proving strategy, the first two option sets are
// the same), plus folded predicates and E150.  dd's preconditions are
// analyzed with diagnose, the rest with prove.  make() puts out the
// virtual function tables, so the inline overriders (E150).
// Mirrors: GCC's p4405-override-nested{,-O0,-nooverride}.C, Clang's
// p4405-override-nested.cpp.

int chk(int v) pre(v != 0);
int chk(int) { return 1; }
namespace dd {
int dchk(int v) pre(v != 0);
int dchk(int) { return 1; }
constexpr int dc(int v) pre(v != 0) { return 1; }
}
constexpr int cc(int v) pre(v != 0) { return 1; }

// B1::f's precondition would make x, hence r, nonzero; nothing proves it
// here.
struct B1 { virtual int f(int x) pre(x > 0) post(r: chk(r) == 1); };
struct D1 : B1 { int f(int x) override { return x; } };

// Unprovable: nothing is known of r.
struct B2 { virtual int f(int x) post(r: chk(r) == 1); };
struct D2 : B2 { int f(int x) override { return x; } };

// Always false: chk(0).
struct B3 { virtual int f(int x) post(r: chk(0) == 1); };
struct D3 : B3 { int f(int x) override { return x; } };

// Diagnose: dd::dchk's precondition, warned at its call in B4::f's
// postcondition, naming the pair; the obligation is reported as well.
struct B4 { virtual int f(int x) post(r: dd::dchk(r) == 1); };
struct D4 : B4 { int f(int x) override { return x; } };

// D5::f's own precondition, whose nested one only B5::f's precondition
// would prove.
struct B5 { virtual int f(int x) pre(x > 0); };
struct D5 : B5 { int f(int x) override pre(chk(x) == 1) { return x; } };

// D6::f's own postcondition: no override obligation, its ordinary analysis
// (with its note).
struct B6 { virtual int f(int x); };
struct D6 : B6 { int f(int x) override post(r: chk(r - 1) == 1) { return x; } };

// D7::f's own precondition with a nested diagnose one.
struct B7 { virtual int f(int x) pre(x > 0); };
struct D7 : B7 { int f(int x) override pre(dd::dchk(x - 1) == 1) { return x; } };

// Folded to true by constant evaluation (cc(0) under ignore), but its nested
// prove evaluation is always false: an obligation (not with no folding:
// "cannot be proven" there).
struct B8 { virtual int f(int x) post(r: cc(0) == 1); };
struct D8 : B8 { int f(int x) override { return x; } };

// Folded to true with a nested diagnose one: no obligation, the warning
// alone.
struct B9 { virtual int f(int x) post(r: dd::dc(0) == 1); };
struct D9 : B9 { int f(int x) override { return x; } };

// E150: the key function is elsewhere, so the inline overrider is not put
// out here: neither the obligation nor its nested warning.
struct BK { virtual int f(int x) post(r: dd::dchk(r) == 1 && chk(r) == 1); };
struct KE : BK {
  int f(int x) override { return x; }
  virtual void key();
};
struct KH : BK {                        // Key function here: both.
  int f(int x) override { return x; }
  virtual void key();
};
void KH::key() { }

void *make(int i) {
  switch (i) {
    case 1: return new D1;
    case 2: return new D2;
    case 3: return new D3;
    case 4: return new D4;
    case 5: return new D5;
    case 6: return new D6;
    case 7: return new D7;
    case 8: return new D8;
    default: return new D9;
  }
}
