//remark:prefix maps: through edg-gxx -ffile-prefix-map= and -fmacro-prefix-map= remap the module name ASan records for a global (g++ builds it), which its description of the global prints, as they remap __FILE__
//require:BACK_END_IS_CP_GEN_BE 1
//options_sep:!
//options:-fsanitize=address -ffile-prefix-map==FILE-PREFIX/;rp!-fsanitize=address -fmacro-prefix-map==MACRO-PREFIX/;rp
//filter:sed -E 's/0x[0-9a-f]+/0xADDR/g'
//match_regex:^__FILE__ is .*$
//match_regex:global variable 'XXX' defined in '[^':]*
// Mirrors: gcc/testsuite/c-c++-common/asan/global-file-prefix-map-1.c and
// global-macro-prefix-map-1.c (gnu_gcc), and
// clang/test/Contracts/Runnable/sanitizer-prefix-map-asan.cpp
// (llvm_llvm-project).  The suite sends ASan's reports to a log file; the
// description goes to stderr.
extern "C" int printf(const char *, ...);
extern "C" int fflush(void *);
extern "C" void __asan_describe_address(void *);
extern "C" void __sanitizer_set_report_path(const char *);

char XXX[10];

int main() {
  __sanitizer_set_report_path("stderr");
  printf("__FILE__ is %s\n", __FILE__);
  fflush(0);
  __asan_describe_address(XXX + 10);
  return 0;
}
