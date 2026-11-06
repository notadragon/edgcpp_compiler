#include <cstdarg>
int f(int n, ...) { va_list a; va_start(a, n); int r = va_arg(a, int); va_end(a); return r; }
