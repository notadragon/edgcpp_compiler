# EDG-136: label facets and P3099 messages do not reach the violation handler under the C-generating back end

**Status:** Open defect; this branch only (contracts are not in upstream
EDG).
**Component:** symbol_tbl.c `enter_contract_noexcept_check_routine` (the
checks call `__c_contract_check_noexcept` with the comment, location, kind
and semantic only), and the lowering that builds those calls.
**Found:** 2026-10-22, with the D3400R5 Compiler Explorer examples (eccp,
`--c++26 --contracts_p3400 --contracts_p4298
--contract_evaluation_semantic=noexcept_observe`, the configuration godbolt's
`edg-notadragon-contracts-p3850-gcc` uses).
**Watch test:**
`tests/tests/contracts/openbugs/edg-136-label-handler-facets/test.mft.cpp`.

## Reproducer

    #include <contracts>
    #include <cstdio>
    struct logged {
      using assertion_control_object = logged;
      static void handle_contract_violation(
          const std::contracts::contract_violation &v) {
        std::printf("local handler: %s\n", v.comment());
      }
    };
    constexpr logged logged_label;
    struct annotated {
      using assertion_control_object = annotated;
      constexpr const char *compute_comment(const char *) const {
        return "computed comment";
      }
      constexpr const char *compute_message(const char *m) const {
        return m ? m : "computed message";
      }
    };
    constexpr annotated annotated_label;
    struct owned {
      using assertion_control_object = owned;
      static constexpr int key = 1;
      void *query(const void *k, std::size_t i) const {
        return k == &key && i == 0 ? (void *)"review_committee" : nullptr;
      }
    };
    constexpr owned owned_label;
    void handle_contract_violation(
        const std::contracts::contract_violation &v) {
      const char *m = v.message();
      const char *o = (const char *)v.query_control_object(&owned::key);
      std::printf("handler: comment \"%s\", message \"%s\", owner \"%s\"\n",
                  v.comment(), m ? m : "(none)", o ? o : "(none)");
    }
    void f(int x) pre<logged_label>(x > 0) { }
    void g(int x) pre<annotated_label>(x > 0) { }
    void h(int x) pre(x > 0, "plain message") { }
    void k(int x) pre<owned_label>(x > 0) { }
    int main() { f(0); g(0); h(0); k(0); }

With eccp (add `--contracts_p3099`):

    handler: comment "x > 0", message "(none)", owner "(none)"
    handler: comment "x > 0", message "(none)", owner "(none)"
    handler: comment "x > 0", message "(none)", owner "(none)"
    handler: comment "x > 0", message "(none)", owner "(none)"

Through edg-gxx (`-fcontracts-p3400 -fcontracts-p3099 -fcontracts-p4298
-fcontract-evaluation-semantic=noexcept_observe`), as with our GCC:

    local handler: (((int)((const int)x)) > 0)
    handler: comment "(((int)((const int)x)) > 0)", message "(none)", owner "(none)"
    handler: comment "computed comment", message "computed message", owner "(none)"
    handler: comment "(((int)((const int)x)) > 0)", message "plain message", owner "(none)"
    handler: comment "(((int)((const int)x)) > 0)", message "(none)", owner "review_committee"

So under the C-generating back end, silently (no diagnostic):

1. `local_violation_label`: the label's `handle_contract_violation` is never
   called.
2. `compute_comment_label` / `compute_message_label`: the computed comment
   and message are not used; the violation's comment is the predicate's
   text and `message()` is null.
3. `queryable_label`: `contract_violation::query_control_object` returns
   null (no query function or label pointer in the violation's data block).
4. A P3099 diagnostic message, label or not, never reaches `message()`
   (M9 planned "nothing" for this back end when only quick_enforce, which
   calls no handler, was available; P4298's noexcept semantics, M13b, call
   the handler but were not extended to it).

Measured 2026-10-22 on the local Compiler Explorer (`edg_p3850_c_installed`
vs `edg_p3850_installed`), on godbolt
(`edg-notadragon-contracts-p3850-gcc`), and in the suite at working
`f2eb1fddab`.  The D3400R5 examples `D3400R5-implexample-handler`,
`-messages` and `-query` (notadragon_wg21 `src/pubs/papers/sg21/`) show the
same.

## Notes

The M11 plan (DECISIONS.md E23 item 8 in notadragon_wg21) accepted labels
under this back end with "handler-side facets ignored until M13", but M13
(P4298) gave the back end a handler call without them, and no row recorded
the gap.  The front end already evaluates `compute_message` (and
`compute_comment`'s constancy) for constant evaluation (M11b,
`resolve_contract_semantics`); what is missing is passing the results, a
message, and the label (its local handler and query function) to the
runtime: the check would build a contracts-ABI data block with the message,
local-handler, query-function and label-pointer fields (as
`__edg_contract_check_noexcept_exception` already builds one for the
location and comment) and dispatch it, instead of calling
`__c_contract_check_noexcept`.  The same block would serve EDG-121's
`evaluation_exception()`.

notadragon_wg21's `src/pubs/impl/p3850impl/edg-overview.md` (M11b/M11c
row) lists compute_message among the facets used "in constant evaluation and
C-generating checks"; the C-generating checks use only the semantic facets
and group names.
