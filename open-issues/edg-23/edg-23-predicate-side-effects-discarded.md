# EDG-23: in constant evaluation a predicate's side effects are discarded; GCC and Clang keep them

**Status:** Divergence by design: the user's decision of 2026-10-01
(notadragon_wg21 `src/pubs/impl/p3850impl/DECISIONS.md`, section U, E8), with
our GCC and Clang to follow (their task rows GCC-631 and CLANG-625).
**Component:** interpret.c (`evaluate_contract_assertion` and the snapshots
of `save_contract_snapshot`).
**Found:** M4, 2026-10-01.

## What differs

    constexpr int f(int i) {
      contract_assert((++const_cast<int &>(i), true));
      return i;
    }
    static_assert(f(1) == 1);   // EDG: holds under every semantic

[basic.contract.eval] Example 1.  In constant evaluation EDG determines a
predicate's value and then undoes whatever the predicate modified, as if an
equivalent evaluation without side effects had been performed, which P2900
permits ("an evaluation ... that produces the same value as the predicate
but has no side effects can occur").  Our GCC and Clang let the modification
stand under every checking semantic (the bound is 2), and diagnose it with
`-Wcontract-constexpr-side-effect`; EDG gives no diagnostic.

## Imports that encode the other behavior

Not imported for this reason (they expect the modification to stand):

* gcc: `basic-contract-eval-example-enforce.C`,
  `contract-assume-nesting-permutations.C`, `contract-constexpr-side-effect.C`,
  `contract-constexpr-side-effect-nested.C`,
  `contract-constexpr-side-effect-warn.C`,
  `contract-nested-modifiable-tracker.C`, `contract-discardable-evaluation.C`
* clang: `contract-constexpr-side-effect.cpp`

The row goes when GCC and Clang discard the modifications too and these
tests change accordingly.
