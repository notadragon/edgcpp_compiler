//remark: imported from gcc:lambda-this-modifies-object.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3098 --contract_evaluation_semantic=enforce
//match_regex: ", line 41: (?:catastrophic )?error
//match_regex: ", line 47: (?:catastrophic )?error
//match_regex: ", line 50: (?:catastrophic )?error
//match_regex: ", line 51: (?:catastrophic )?error
//match_regex: ", line 55: (?:catastrophic )?error
//match_regex: ", line 56: (?:catastrophic )?error
//match_regex: ", line 57: (?:catastrophic )?error
//match_regex: ", line 58: (?:catastrophic )?error
//match_regex: ", line 59: (?:catastrophic )?error
//match_regex: ", line 60: (?:catastrophic )?error
//match_regex: ", line 62: (?:catastrophic )?error
//match_regex: ", line 63: (?:catastrophic )?error
//match_regex: ", line 64: (?:catastrophic )?error
//match_regex: ", line 68: (?:catastrophic )?error
//match_regex: ", line 69: (?:catastrophic )?error
// Within a contract predicate `this' is a pointer to const ([expr.prim.this]):
// explicitly, through an implicit member access, and where a lambda in the
// predicate captures it -- so neither the predicate nor such a lambda may
// modify the object through it (GCC-641; stock GCC too), nor through a
// postcondition capture (P3098) initialized from it.  A lambda's copy of
// *this is its own member, which a mutable lambda may modify
// ([expr.prim.id.unqual]).
//
// The rest of a postcondition capture's initializer is constified too: an
// entity declared outside the contract assertion may not be modified there
// (D3098R3: `post [j = ++i] (j)' is an error).  The capture itself is
// deduced as `auto' would be, without the initializer's const, and may be
// modified in the predicate (GCC-662).
//
// Mirrors: clang/test/Contracts/lambda-this-modifies-object.cpp in the
// llvm_llvm-project fork, and p3098-capture-init-constified.cpp there for
// the captures (CLANG-641).
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3098 -fcontract-evaluation-semantic=enforce" }

int gi = 0;
void g () post [j = ++gi] (j > 0);	// { dg-error "read-only" }

struct S
{
  int m;
  static void sf (S *) {}
  void f () pre ([this] { return ++m > 0; } ());	// { dg-error "read-only" }
  void g ()
  {
    contract_assert ([this] { return ++m > 0; } ());	// { dg-error "read-only" }
    contract_assert ([&] { return ++m > 0; } ());	// { dg-error "read-only" }
    contract_assert ([this] { return m > 0; } ());	// OK, a read
    contract_assert ([*this] () mutable { return ++m > 0; } ());	// OK, the copy
  }
  void h () pre ([=, this] { return ++m > 0; } ());	// { dg-error "read-only" }
  void i () pre ([p = this] { return ++p->m > 0; } ());	// { dg-error "read-only" }
  void j () pre ([&] { return [&] { return ++m > 0; } (); } ());	// { dg-error "read-only" }
  void k () pre ([this] { return [=, this] () mutable { return ++m > 0; } (); } ()); // { dg-error "read-only" }
  void n () pre ((sf (this), true));			// { dg-error "invalid conversion" }
  void o () pre ((++this->m, true));			// { dg-error "read-only" }
  // In a lambda's body, a contract_assert's `this' is the enclosing one.
  void q () { [this] { contract_assert (++m > 0); } (); }	// { dg-error "read-only" }
  void r () { [this] { contract_assert ((sf (this), true)); } (); }	// { dg-error "invalid conversion" }
  void s () { [this] { contract_assert ([&] { return ++m > 0; } ()); } (); }	// { dg-error "read-only" }
  // A postcondition capture's initializer is in the contract assertion too
  // (P3098): the capture is a pointer to const.
  void t () post [p = this] (p != nullptr);		// OK
  void u () post [p = this] (++p->m > 0);		// { dg-error "read-only" }
  void v () post [q = ++m] (q > 0);			// { dg-error "read-only" }
};

int k (int y) post [old = y] (++old > 1);		// OK, not constified
int k2 (const int y) post [old = y] (++old > y);	// OK, deduced as int
int k3 (const int y)
  post [old = y] ([] { static_assert (__is_same (decltype (old), int)); return true; } ());
