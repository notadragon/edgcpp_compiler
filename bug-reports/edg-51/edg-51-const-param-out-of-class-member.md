# EDG-51: an out-of-class member definition of a class template takes its parameters' top-level const from the declaration

**Status:** Open upstream; fixed on this branch (band 0100,
`is_ordinary_member_of_class_instance` in declarator.c and
`copy_param_cv_qualifiers_from_proto`'s call in templates.c).  Not
contracts-specific.
**Component:** the matching of an out-of-class definition of a member
function of a class template to its in-class declaration (the definition's
parameter variables get the declaration's parameter types).
**Upstream Link:** None found (searched 2026-10-01).
**Affects:** stock EDG at `90b66472e0` (`cpfe --c++17 --no_code_gen`) and
this branch.
**Found:** 2026-10-01, while fixing EDG-27 (agents' cross-checks).
**Test:** `tests/tests/contracts/sema/param_cv_member_definition.sft.cpp`.

## Bug Report

Top-level cv-qualifiers of a parameter are not part of the function type
([dcl.fct]/5); they apply only to the parameter variable in the declaration
that is a definition.  For a member function of a class template declared
with a `const` parameter in the class and defined outside it without
`const`, EDG treats the definition's parameter as const.

## Reproducer

[`const-param-out-of-class-member.cpp`](const-param-out-of-class-member.cpp):

    cpfe --c++17 --no_code_gen const-param-out-of-class-member.cpp

    "const-param-out-of-class-member.cpp", line 5: error: expression must be a modifiable lvalue
              detected during instantiation of "void C<T>::m(T) [with T=int]" at
                        line 6

Our GCC accepts it.  A function that is not a member of a template is
right: `void g(const int x); void g(int x) { x = 1; }` is accepted.

## Notes

Consequence for contracts on this branch: EDG-27's rule (a non-reference
parameter a postcondition odr-uses must be const in every declaration of
the function) cannot be checked for such members: the definition's
parameter has the in-class declaration's type, not its own.

A member function template of a class that is not a template has the same
bug by another path (found while fixing it, 2026-10-02, also on stock):
`struct S { template <class U> void m(const U x); };
template <class U> void S::m(U x) { x = 1; } template void S::m(int);`
is rejected.

## Fix (this branch)

* The in-class declaration's top-level qualifiers stayed in the
  param-type entries (`ptp->qualifiers`) of a member of a class template,
  both in the template itself (where an operand of non-dependent type is
  checked when the definition is parsed: the imported Clang test
  `SemaTemplate/dependent-type-identity` got a false error there) and in
  each instance, whose member is declared by rescanning the in-class
  declaration; `decl_parameter` ORs the definition's into those.  The scan
  of such a declaration (not a member template or a friend) now drops them, as the rescan of a function template's
  redeclaration for substitution already did
  (`must_adjust_param_type_qualifiers` in `function_declarator`).  Const
  from a template argument is not written in the declaration, so it stays.
* `copy_param_cv_qualifiers_from_proto` copies the qualifiers of the
  template's own type to each instance; for a namespace template they are
  the definition's, for a member template the in-class declaration's.  It
  is now skipped for class members; `decl_parameter` adds the definition's.
