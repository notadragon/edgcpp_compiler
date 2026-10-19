//remark:contracts: a function whose body is a function-try-block checks its preconditions and postconditions (EDG-83)
//options_all:--contract_evaluation_semantic=quick_enforce
//options:-DVIOLATE=0;rp:-DVIOLATE=1;rn:-DVIOLATE=2;rn
//match_regex:^start$
// (No system headers: also runs under the C-generating back end.)
extern "C" int puts(const char *);
extern "C" int fflush(void *);
int f(int x) pre (x > 0) try { return x; } catch (...) { return -1; }
int g(const int x) post (r: r == x) try {
  return x == 5 ? 6 : x;
} catch (...) {
  return -1;
}
int main() {
  puts("start");
  fflush(0);
  if (VIOLATE == 0) { f(1); g(1); }
  if (VIOLATE == 1) f(0);
  if (VIOLATE == 2) g(5);
  puts("end");
  return 0;
}
