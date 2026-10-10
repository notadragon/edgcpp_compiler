# EDG-26: `this` in a lambda body in a contract predicate points to const

**Status:** Divergence between our GCC and Clang; EDG follows Clang.
**Component:** overload.c (`variable_this_exists_full`,
`make_this_variable_operand`), exprutil.c (`in_lambda_in_contract_predicate`).
**Found:** M3b, 2026-10-01.

## Example

    struct S {
      int m;
      void h() { contract_assert([&] { return ++m > 0; }()); }
    };

EDG and our Clang reject the increment (Clang: "cannot assign to non-static
data member because it is considered 'const' inside of a contract"); our GCC
accepts it.  All three reject `++x` for a local `x` captured by reference
there.

## Notes

P2900 [expr.prim.this] makes `this` a pointer to const where it "appears
within the predicate of a contract assertion"; the lambda body is part of
the predicate's expression, so EDG and Clang read the rule as applying
there.  GCC constifies `*this` only in the predicate itself
(`current_class_ref`), and inside a lambda only the variables it names.

The same holds in a lambda nested in such a lambda, at any depth
([expr.prim.this]: "including in the bodies of nested lambda-expressions");
Clang agrees, our GCC accepts the modification there too.
