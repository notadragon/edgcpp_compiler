---
id: 0130-param-cv-from-definition
subject: 'Take a parameter variable''s top-level cv-qualifiers from the definition []'
depends: []
regenerates: []
fixes: []
---

## Rationale

A fix to upstream EDG, independent of contracts.  Top-level cv-qualifiers of
a parameter are not part of the function type ([dcl.fct]/5) and apply only
to the parameter variables of the definition.  Two kinds of member defined
outside its class took them from the in-class declaration as well:

* A member of a class template.  The in-class declaration's qualifiers
  stayed in its parameter types, both in the class template (where an
  operand of non-dependent type is checked when the definition is parsed)
  and in each instance, whose member is declared by rescanning the in-class
  declaration.  They are now dropped there (`function_declarator`, as for
  the redeclaration of a function template rescanned for substitution), and
  `decl_parameter` adds the definition's; const from a template argument
  stays.
* A member template.  `copy_param_cv_qualifiers_from_proto` copies the
  qualifiers of the template's type to each instance, which for a namespace
  template are the definition's but for a member are the in-class
  declaration's; it is now skipped for members.

So assigning to such a parameter was rejected, and with contracts the rule
that a postcondition's value parameters be const in every declaration could
not be checked for the definition.  The recording of the imported Clang test
`SemaTemplate/dependent-type-identity` held the bug (three errors for
`void A<T>::f(int x) { x = 0; }` after `void f(const int);` in the class,
which Clang's test expects to be accepted) and is updated.

Tracked in `bug-reports/` (EDG-51) until upstream fixes it.

## Compile gap

None.

## Contents

- src/declarator.c : #0, #1, #2, #3
- src/templates.c : #3
- tests/tests/contracts/sema/param_cv_member_definition.sft.cpp : *
- tests/tests/contracts/sema/.param_cv_member_definition.rto/* : *
- tests/expectations/edg_x86_64/contracts.log : /^sema\/param_cv_member_definition/
- tests/tests/imported/clang/cpp/SemaTemplate/.dependent-type-identity.rto/* : *
