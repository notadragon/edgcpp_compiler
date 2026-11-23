// The operand of an assumption is not evaluated: f must return 0.
// cpfe --c++23 --gen_c_file_name=t.int.c assume-operand-inlined.cpp
// gcc -x c t.int.c -o t && ./t; echo $?      (prints 1)
inline bool g(int *p) { if (*p == 0) ++*p; return true; }
int f() { int i = 0; [[assume(g(&i))]]; return i; }
int main() { return f(); }
