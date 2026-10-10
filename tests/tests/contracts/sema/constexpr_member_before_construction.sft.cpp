//remark:constant evaluation rejects referring to a member of a member whose constructor has not begun
//type:fn
//match_regex:line 16: error: expression must have a constant value
//match_regex:line 19: error: expression must have a constant value
//match_regex:line 21: error: expression must have a constant value
//require:BACK_END_IS_CP_GEN_BE 1
// Not contracts-specific (an upstream fix, EDG-45): for an object with a
// non-trivial constructor, referring to a non-static member before the
// constructor begins is undefined behavior ([class.cdtor]/1).  Forming the
// address of the object itself is fine ([class.cdtor]/3), as is a member
// whose constructor is trivial, or one already constructed.  GCC and Clang
// accept the first case (GCC-2).
struct Z { int k; constexpr Z() : k(0) { } };
struct Y { int *p; Z z; constexpr Y() : p(&z.k) { } };
constexpr int f() { Y y; return y.p != nullptr; }
static_assert((f(), true));                     // Error
struct Y2 { int q; Z z; constexpr Y2() : q(z.k) { } };
constexpr int f2() { Y2 y; return y.q; }
static_assert((f2(), true));                    // Error
struct Y3 { Z *p; int *r; Z z; constexpr Y3() : p(&z), r(&p->k) { } };
static_assert((Y3(), true));                    // Error

struct Y4 { Z *p; Z z; constexpr Y4() : p(&z) { } };  // OK, &z itself
static_assert((Y4(), true));
struct T { int k; };
struct Y5 { int *p; T t; constexpr Y5() : p(&t.k), t{1} { } };  // OK, T trivial
static_assert((Y5(), true));
struct Y6 { Z z; int *p; constexpr Y6() : p(&z.k) { } };  // OK, z constructed
static_assert((Y6(), true));
struct Y7 { Z z; int k; constexpr Y7() : k(0) { z.k = 1; } };  // OK, in the body
static_assert((Y7(), true));
