//remark:constant evaluation rejects destroying an object twice and writing to an object after its destruction
//type:fn
//match_regex:line 19: error: expression must have a constant value
//match_regex:line 24: error: expression must have a constant value
//match_regex:line 26: error: expression must have a constant value
//match_regex:line 29: error: expression must have a constant value
//match_regex:line 34: error: expression must have a constant value
//match_regex:line 38: error: expression must have a constant value
//match_regex:line 42: error: expression must have a constant value
//require:BACK_END_IS_CP_GEN_BE 1
// Not contracts-specific (an upstream fix, EDG-44): ending the lifetime of an
// object that is not within its lifetime, or storing to it, is undefined
// behavior ([basic.life]), so not a core constant expression.  The storage of
// a destroyed object can be reused: constructing in it, or a store that
// starts the lifetime of a union member, is fine.
#include <memory>
struct S { float f = 0; constexpr ~S() {} };
constexpr bool destroy_then_delete() { S *p = new S; p->~S(); delete p; return true; }
static_assert(destroy_then_delete());           // Error
constexpr bool write_after_destroy() {
  S *p = new S; p->~S(); p->f = 1; delete p;
  return true;
}
static_assert(write_after_destroy());           // Error
constexpr bool local_twice() { S s; s.~S(); return true; }
static_assert(local_twice());                   // Error
struct T { int i = 0; };
constexpr bool trivial_twice() { T t; t.~T(); t.~T(); return true; }
static_assert(trivial_twice());                 // Error
constexpr bool assign_after_destroy() {
  T *p = new T; p->~T(); *p = T{}; delete p;
  return true;
}
static_assert(assign_after_destroy());          // Error
constexpr bool explicit_twice() {
  S *p = new S; p->~S(); p->~S(); return true;
}
static_assert(explicit_twice());                // Error
constexpr bool write_after_local_destroy() {
  S s; s.~S(); s.f = 1; return true;
}
static_assert(write_after_local_destroy());     // Error

constexpr bool reconstruct() {
  S s;
  s.~S();
  std::construct_at(&s);
  s.f = 2;
  return s.f == 2;                              // OK, destroyed again at '}'
}
static_assert(reconstruct());
constexpr bool reconstruct_heap() {
  S *p = new S;
  p->~S();
  std::construct_at(p);
  delete p;                                     // OK
  return true;
}
static_assert(reconstruct_heap());
union U { int i; float f; };
constexpr bool union_member() {
  U u{1};
  u.f = 2;                                      // OK, starts f's lifetime
  return u.f == 2;
}
static_assert(union_member());
constexpr bool member_destroyed() {
  struct P { S a, b; };
  P p;
  p.a.~S();                                     // A subobject: not tracked
  std::construct_at(&p.a);
  return true;
}
static_assert(member_destroyed());
constexpr bool array_elements() {
  S *p = new S[2];
  p[0].~S();
  std::construct_at(&p[0]);
  delete[] p;
  return true;
}
static_assert(array_elements());
