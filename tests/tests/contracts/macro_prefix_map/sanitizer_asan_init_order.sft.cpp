//remark:prefix maps: through edg-gxx -ffile-prefix-map= remaps the module name of a translation unit's globals and the one passed to __asan_before_dynamic_init alike, so its own dynamic initialization is not reported as an initialization-order fiasco
//require:BACK_END_IS_CP_GEN_BE 1
//type:rp
//options:-fsanitize=address -ffile-prefix-map==FILE-PREFIX
// Mirrors: gcc/testsuite/g++.dg/asan/init-order-file-prefix-map.C (gnu_gcc)
// and clang/test/Contracts/Runnable/sanitizer-prefix-map-asan-init-order.cpp
// (llvm_llvm-project).  The suite sets ASAN_OPTIONS (a report goes to a log
// file and the exit status is 139) without the check, so
// __asan_default_options turns it on.
extern "C" const char *__asan_default_options() {
  return "check_initialization_order=true";
}

struct S {
  S() { asm volatile("" : : : "memory"); }
};
static S c;
static int a = 0;
static int b = a;

int main() { return b; }
