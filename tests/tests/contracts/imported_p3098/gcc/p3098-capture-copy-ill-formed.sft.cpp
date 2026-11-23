//remark: imported from gcc:p3098-capture-copy-ill-formed.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3098
//match_regex: ", line 38: (?:catastrophic )?error
//match_regex: ", line 39: (?:catastrophic )?error
//match_regex: ", line 41: (?:catastrophic )?error
//match_regex: ", line 43: (?:catastrophic )?error
// P3098: a capture is copy-initialized from its initializer, so a type that
// cannot be copy-initialized is ill-formed.  Deleted, inaccessible, and
// explicit-only copy constructors each make the capture ill-formed; the
// explicit case is what pins copy-initialization rather than
// direct-initialization.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3098" }

bool ok (int);

struct Deleted {
  int v = 0;
  Deleted () = default;
  Deleted (const Deleted &) = delete;
};

struct Private {
  int v = 0;
  Private () = default;
private:
  Private (const Private &) = default;
};

struct Explicit {
  int v = 0;
  Explicit () = default;
  explicit Explicit (const Explicit &) = default;
};

void deleted_param (Deleted p) post [p] (ok (p.v)) { }	    // { dg-error "deleted|use of deleted" }
void deleted_init (Deleted p) post [q = p] (ok (q.v)) { }   // { dg-error "deleted|use of deleted" }

void private_param (Private p) post [p] (ok (p.v)) { }	    // { dg-error "private" }

void explicit_param (Explicit p) post [p] (ok (p.v)) { }    // { dg-error "explicit|no matching function" }
