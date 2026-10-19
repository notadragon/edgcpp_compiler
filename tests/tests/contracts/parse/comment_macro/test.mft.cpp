//remark:contracts: the comment of a contract whose predicate involves macros is its tokens after macro expansion
//require:DO_IL_LOWERING 1
//type:fp
//source_files:comment_macro.h
//options_all:--no_il_lowering --il_display --contract_evaluation_semantic=ignore
//filter:awk '/^kind: +ctk_/ { k = $2 } /^comment: / { sub(/^comment: +[^ ]+ other-text@[^ ]+ /, ""); print k, $0 }'
// Mirror of our GCC's g++.dg/contracts/cpp26/contract-comment-macro.C (and
// contract-comment-macro.h; the gnu_gcc fork's GCC-707: g++'s comment was
// wrong when a predicate began or ended in a macro expansion) and of
// Clang's clang/test/Contracts/contract-comment-macro.cpp, with the same
// shapes.  GCC and Clang give source text; the front end forms the comment
// from the predicate's tokens after macro expansion (scan_contract_predicate),
// separated by one space, whatever the shape: a macro at either end, from a
// header, invoked over two lines, with a comment or a string in the
// invocation, a pasted token, the whole contract from a macro, and tokens
// from two arguments or one argument twice.  GCC's
// contract-comment-macro-untracked.C (-ftrack-macro-expansion=0, where g++
// too uses the tokens, spelling a literal from its value) adds a literal
// written in hex on a second line; the front end keeps its spelling.  The
// comment is implementation-defined; this pins the front end's own, seen
// in the IL (C-generating back end only: through edg-gxx every comment is
// g++'s printed expression, see comment_gxx).  The filter prints each
// contract's kind and comment, in the order of the IL display (a class's
// member functions come first).
#include "comment_macro.h"

#define X x
#define ZERO 0
#define Y X
#define NEG(a) ((a) < 0)
#define ID(e) e
#define LHS x <
#define RHS < 0
#define FIRST(a, b) (a)
#define CAT(a, b) a ## b
#define PRE_NEG pre(x < 0)
#define MY_PRE(c) pre(c)
#define MY_POST(c) post(r: c)
#define MY_ASSERT(c) contract_assert(c)
#define WRAP(c) MY_PRE(c)
#define CHK(c) contract_assert((c) > 0)
#define BOTH(a, b) pre(a && b)
#define TWICE(a) pre(a == -a)

// No macro: a literal keeps its spelling.
int plain(int x) pre(x
                     < 0x10) { return x; }

// Outside any expansion.
int begins_in(int x) pre(X < 0) { return x; }
int begins_in_fn(int x) pre(NEG(x) && x > 9) { return x; }
int ends_in(int x) pre(x < ZERO) { return x; }
int ends_in_fn(int x) pre(x > 9 || NEG(x)) { return x; }
int two(int x) pre(X < ZERO) { return x; }
int two_fn(int x) pre(NEG(x) || NEG(x)) { return x; }
int one_fn(int x) pre(NEG(x)) { return x; }
int first_half(int x) pre(LHS 0) { return x; }
int second_half(int x) pre(x RHS) { return x; }
int header(int x) pre(HX < 0) { return x; }
int nested(int x) pre(Y < 0) { return x; }
int in_argument(int x) pre(ID(x) < 0) { return x; }
int whole_argument(int x) pre(ID(x < 0)) { return x; }
int multi_line(int x) pre(NEG(
                            x)) { return x; }
int with_comment(int x) pre(NEG(x /* why */)) { return x; }
int string_argument(int x) pre(FIRST(x < 0, ")")) { return x; }
int pasted(int xy) pre(CAT(x, y) < 0) { return xy; }

// The whole contract from a macro.
int body(int x) PRE_NEG { return x; }
int argument(int x) MY_PRE(x < 0) { return x; }
int argument_two(int x) MY_PRE(X < ZERO) { return x; }
int argument_fn(int x) MY_PRE(NEG(x)) { return x; }
int argument_wrapped(int x) WRAP(x < 0) { return x; }
int argument_post(int x) MY_POST(r < 0) { return x; }

// Tokens from more than one place.
int two_arguments(int x) BOTH(x > 0, x < 5) { return x; }
int argument_twice(int x) TWICE(x) { return x; }

struct S {
  int member(int x) pre(NEG(x)) { return x; }
  int member_argument(int x) MY_PRE(X < ZERO) { return x; }
};

void f() {
  S s;  // The member functions are used, or the IL drops them as unneeded.
  s.member(5);
  s.member_argument(5);
  int x = 5;
  contract_assert(X < ZERO);
  MY_ASSERT(x < 0);
  CHK(-x);
}
