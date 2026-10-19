//remark:contracts: a predicate whose only potentially-throwing part is a call it makes implicitly -- the destructor a delete-expression invokes, or the default constructor of the array elements an initializer list leaves to its array filler -- is still evaluated under the check's handler, so under noexcept_observe its exception is reported with the evaluation_exception detection mode (2) and does not escape the function
//options_all:--contracts_p4298 --contract_evaluation_semantic=noexcept_observe
//options:;rp
//match_regex:^(kind [123] semantic [67] mode 2|caught|end)$
// (No system headers: also runs under the C-generating back end, whose
// check decides by expr_might_throw whether to build the handler.)  Mirrors
// Clang's predicate-throw-delete.cpp and predicate-throw-array-filler.cpp
// (CLANG-665, CLANG-664) and GCC's predicate-throw-delete.C and
// predicate-throw-array-filler.C, whose imports run only under gxx.
extern "C" int puts(const char *);
extern "C" int printf(const char *, ...);
extern "C" int fflush(void *);
extern "C" int stdc_contract_violation_kind(const void *);
extern "C" int stdc_contract_violation_semantic(const void *);
extern "C" int stdc_contract_violation_detection_mode(const void *);
namespace std { namespace contracts { class contract_violation; } }

void handle_contract_violation(const std::contracts::contract_violation &v) {
  printf("kind %d semantic %d mode %d\n", stdc_contract_violation_kind(&v),
         stdc_contract_violation_semantic(&v),
         stdc_contract_violation_detection_mode(&v));
  fflush(0);
}

struct D { ~D() noexcept(false) { throw 1; } };
bool f(D *p) pre ((delete p, true)) { return true; }

struct S { S() { throw 2; } };
struct A { S s[2]; };
bool g(int x) pre (((void)A{}, x > 0)) { return true; }

int main() {
  try { f(new D); g(1); } catch (...) { puts("caught"); }
  puts("end");
  return 0;
}
