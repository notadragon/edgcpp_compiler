//remark:contracts: EDG-78: the C-generating back end drops extern inline from a gnu_inline function, so gcc compiles an out-of-line copy in which __builtin_va_arg_pack is invalid
//require:DO_IL_LOWERING 1
//type:rp
//options_all:--gnu_version=170000
// Records the current failure; a fix shows up as a deviation.
extern int g(int, ...);
extern inline __attribute__((always_inline, gnu_inline)) int
f(int a, ...)
{
  return g(a, __builtin_va_arg_pack());
}
int g(int a, ...) { return a; }
int main() { return f(0, 1, 2); }
