//remark:contracts: the operand of an assumption is not evaluated at run time
//type:rp
//options_all:--contract_evaluation_semantic=quick_enforce
// [dcl.attr.assume]: the operand of an assumption is never evaluated, so the
// side effects of the calls below (and the contract assertion in one of
// them) never happen.  The C-generating back end used to inline such a call
// at the top of the operand by overwriting the assumption with the inlined
// code, which then ran, and a Debug build found the attribute's file-scope
// IL unwritten (EDG-52).  Runs with both back ends.  (No system headers: the
// C-generating configuration cannot find them.)
extern "C" int puts(const char *);

inline bool bump(int *p) { if (*p == 0) ++*p; return true; }
constexpr bool bump_checked(int *p) {
  contract_assert(*p == 0);
  ++*p;
  return true;
}

int plain() { int i = 0; [[assume(bump(&i))]]; return i; }
constexpr int checked() { int i = 0; [[assume(bump_checked(&i))]]; return i; }

int (*volatile p_checked)() = checked;

int main() {
  int failures = 0;
  if (plain() != 0) { puts("plain: operand evaluated"); ++failures; }
  if (p_checked() != 0) { puts("checked: operand evaluated"); ++failures; }
  puts("end");
  return failures;
}
