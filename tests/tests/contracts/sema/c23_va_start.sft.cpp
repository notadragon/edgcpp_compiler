//remark:contracts: the C++26 va_start of GCC 16+ <cstdarg> (EDG-67)
//require:BACK_END_IS_CP_GEN_BE 1
//type:rp
//options_all:--c++26
// In C++26 mode, the va_start macro of GCC 16 and later expands to
// __builtin_c23_va_start, whose second argument is optional and ignored.
#include <cstdarg>
#include <cstdio>

int first(int n, ...) {
  va_list ap;
  va_start(ap, n);                            // OK, the C++23 form
  int r = va_arg(ap, int);
  va_end(ap);
  return r;
}

int sum(int n, ...) {
  va_list ap, cp;
  va_start(ap);                               // OK, the C++26 form
  va_copy(cp, ap);
  int r = 0;
  for (int i = 0; i < n; ++i) r += va_arg(ap, int);
  for (int i = 0; i < n; ++i) r += va_arg(cp, int);
  va_end(cp);
  va_end(ap);
  return r;
}

int only_ellipsis(...) {
  va_list ap;
  va_start(ap);                               // OK, no named parameter
  int r = va_arg(ap, int);
  va_end(ap);
  return r;
}

template <class T> T templ(int, ...) {
  va_list ap;
  va_start(ap);                               // OK
  T r = va_arg(ap, T);
  va_end(ap);
  return r;
}

struct S {
  int m(...) {
    va_list ap;
    va_start(ap);                             // OK
    int r = va_arg(ap, int);
    va_end(ap);
    return r;
  }
};

int main() {
  int bad = 0;
  if (first(1, 42) != 42) bad |= 1;
  if (sum(2, 20, 1) != 42) bad |= 2;
  if (only_ellipsis(42) != 42) bad |= 4;
  if (templ<long>(0, 42L) != 42) bad |= 8;
  if (S().m(42) != 42) bad |= 16;
  std::printf("%d\n", bad);
  return bad;
}
