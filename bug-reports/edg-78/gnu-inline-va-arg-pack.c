extern int g(int, ...);
extern inline __attribute__((always_inline, gnu_inline)) int
f(int a, ...)
{
  return g(a, __builtin_va_arg_pack());
}
int g(int a, ...) { return a; }
int main(void) { return f(0, 1, 2); }
