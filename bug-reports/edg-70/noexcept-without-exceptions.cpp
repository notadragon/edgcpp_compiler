void f();
static_assert(!noexcept(f()));   // g++ -fno-exceptions: OK; EDG: fails
