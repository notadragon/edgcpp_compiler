//remark:contracts: P3098: a lambda in a postcondition's predicate names a capture, with the C-generating back end (EDG-75)
//require:DO_IL_LOWERING 1
//options_all:--contracts_p3098 --contract_evaluation_semantic=quick_enforce
//options:-DBAD=0;rp:-DBAD=1;rn
//match_regex:^start$
// (No system headers.)  Only the C-generating back end: our GCC stops with
// an internal error on such a lambda (staged in notadragon_wg21's
// p3850impl/TODO.md).
extern "C" int puts(const char *);
extern "C" int fflush(void *);
void say(const char *s) { puts(s); fflush(0); }
int twice(int &x) post [old = x] ([=] { return x == old * 2 + BAD; }()) {
  x *= 2;
  return x;
}
int main() {
  say("start");
  int i = 3;
  twice(i);
  say("end");
}
