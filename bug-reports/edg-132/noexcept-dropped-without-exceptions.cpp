void g() noexcept {}
void (*p)() noexcept = &g;     // g++ -fno-exceptions: OK; through cpfe-cp: error
static_assert(noexcept(g()));  // g++ -fno-exceptions: OK; through cpfe-cp: error
