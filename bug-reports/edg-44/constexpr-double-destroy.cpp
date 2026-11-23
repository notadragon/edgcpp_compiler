// Constant evaluation must reject each of these: the object's lifetime has
// ended (by the explicit destructor call) before it is destroyed again or
// written to.  All three static_asserts should fail.
struct S { float f = 0; constexpr ~S() {} };
constexpr bool destroy_then_delete() { S *p = new S; p->~S(); delete p; return true; }
static_assert(destroy_then_delete());
constexpr bool write_after_destroy() { S *p = new S; p->~S(); p->f = 1; delete p; return true; }
static_assert(write_after_destroy());
constexpr bool local_twice() { S s; s.~S(); return true; }
static_assert(local_twice());
