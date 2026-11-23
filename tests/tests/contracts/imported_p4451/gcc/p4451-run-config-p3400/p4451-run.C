// P4451: every evaluation of a terminating contract assertion uses a
// terminating semantic ([basic.contract.eval]); a configured ignore or
// observe becomes enforce, as for P3400's controls::enforced.  Each kind
// of assertion: pre, post with and without a result name, contract_assert,
// a member function, a function template, a lambda, a P3097 virtual
// function, and with a P3098 capture, a P3099 message and a P4283
// requires-clause.  The handler checks the semantic and throws, so each
// violation is counted.  Run under -fcontracts-p4451 alone; the -p3400 and -both
// files include this one.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts-p4451 -fcontracts-p3097 -fcontracts-p3098 -fcontracts-p3099 -fcontracts-p4283" }
// { dg-additional-options "-fcontract-evaluation-semantic=ignore" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <contracts>
#include <cstdlib>

namespace scl = std::contracts::controls;

using std::contracts::evaluation_semantic;

static int handled;

void handle_contract_violation (const std::contracts::contract_violation &v)
{
  if (v.semantic () != evaluation_semantic::enforce)
    std::abort ();
  ++handled;
  throw handled;
}

int pre_f (int x) pre<scl::enforced> (x > 0) { return x; }
int post_f (const int x) post<scl::enforced> (x > 0) { return x; }
int post_r (int x) post<scl::enforced> (r : r > 0) { return x; }
void assert_f (int x) { contract_assert<scl::enforced> (x > 0); }

struct S
{
  int mem (int x) pre<scl::enforced> (x > 0) { return x; }
  static int smem (int x) pre<scl::enforced> (x > 0);
  virtual int virt (int x) pre<scl::enforced> (x > 0) { return x; }
};
int S::smem (int x) pre<scl::enforced> (x > 0) { return x; }

template<typename T>
T tmpl (const T x) pre<scl::enforced> (x > 0) post<scl::enforced> (r : r == x)
{ return x; }

template<typename T>
T req (T x) pre<scl::enforced> requires (sizeof (T) > 1) (x > 0) { return x; }

int cap (int x) post<scl::enforced> [x0 = x] (r : r == x0 && x0 > 0)
{ return x; }

int msg (int x) pre<scl::enforced> (x > 0, "x must be positive") { return x; }

auto lam = [] (int x) pre<scl::enforced> (x > 0) { return x; };

#define CHECK(E) \
  do { try { (void) (E); std::abort (); } catch (int) { } } while (0)

int
main ()
{
  S s;
  S &rs = s;
  CHECK (pre_f (-1));
  CHECK (post_f (-1));
  CHECK (post_r (-1));
  CHECK ((assert_f (-1), 0));
  CHECK (s.mem (-1));
  CHECK (S::smem (-1));
  CHECK (rs.virt (-1));
  CHECK (tmpl (-1));
  CHECK (req (-1));
  CHECK (cap (-1));
  CHECK (msg (-1));
  CHECK (lam (-1));
  int expected = handled;
  /* Satisfied assertions do not call the handler.  */
  pre_f (1); post_f (1); post_r (1); assert_f (1); s.mem (1); S::smem (1);
  rs.virt (1); tmpl (1); req (1); cap (1); msg (1); lam (1);
  if (handled != expected || expected < 12)
    std::abort ();
}
