// Constant evaluation must reject this: Y's constructor forms &z.k before
// z's (non-trivial) constructor has begun, which is undefined behavior
// ([class.cdtor]/1).  The static_assert should fail.
struct Z { int k; constexpr Z() : k(0) { } };
struct Y { int *p; Z z; constexpr Y() : p(&z.k) { } };
constexpr int f() { Y y; return y.p != nullptr; }
static_assert((f(), true));
