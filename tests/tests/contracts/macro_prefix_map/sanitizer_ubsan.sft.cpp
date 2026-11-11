//remark:prefix maps: through edg-gxx -ffile-prefix-map= and -fmacro-prefix-map= remap the file name of a UBSan report (g++ builds it) as they remap __FILE__
//require:BACK_END_IS_CP_GEN_BE 1
//options_sep:!
//options:-fsanitize=integer-divide-by-zero -ffile-prefix-map=/src=/mapped;rp!-fsanitize=integer-divide-by-zero -fmacro-prefix-map=/src=/mapped;rp
//match_regex:^(__FILE__ is .*|/mapped/repro\.cpp:[0-9]+:[0-9]+: runtime error: division by zero)$
// Mirrors: gcc/testsuite/c-c++-common/ubsan/file-prefix-map-1.c and
// macro-prefix-map-1.c (gnu_gcc), and
// clang/test/Contracts/Runnable/sanitizer-prefix-map-ubsan.cpp
// (llvm_llvm-project).  The #line directive gives the file a fixed name to
// map.
extern "C" int printf(const char *, ...);
extern "C" int fflush(void *);

#line 1 "/src/repro.cpp"
int main() {
  volatile int a = 1, b = 0;
  printf("__FILE__ is %s\n", __FILE__);
  fflush(0);
  (void)(a / b);
  return 0;
}
