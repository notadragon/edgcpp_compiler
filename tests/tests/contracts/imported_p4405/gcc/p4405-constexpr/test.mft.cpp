//remark: imported from gcc:p4405-constexpr.C
//type: fc
//require: BACK_END_IS_CP_GEN_BE 1
//source_files: p4405-constexpr.json
//options: --c++26 --contracts --contracts_p4405 --contract_configuration_file=p4405-constexpr.json
//match_regex: ^(Command-line error: invalid contract configuration .*)$
// EDG: adapted -- the front end's first configuration error ends the
// compilation (EDG reports it as a command-line error), so only the first
// of the original's two errors appears, and the source is not compiled.
// P4405: constant evaluation is never analyzed (D4405, DECISIONS.md E142),
// so an entry selecting prove or diagnose for constant evaluation alone
// ("constexpr": true) is a configuration error, and is skipped.  Here the
// next entry gives constant evaluation ignore and the run time ignore, so a
// false predicate is not evaluated, the constant expression does not fail,
// and nothing else is diagnosed.  A prove or diagnose that constant
// evaluation reaches otherwise is enforce or observe there
// (p4405-constexpr-enforce.C, p4405-constexpr-diagnose.C).
//
// Mirror: clang/test/Contracts/p4405-constexpr.cpp in the llvm_llvm-project
// fork.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p4405" }
// { dg-additional-options "-fcontract-configuration-file=${srcdir}/g++.dg/contracts/cpp26/p4405-constexpr.json" }
//
// The diagnostics point into the JSON file, so match them with dg-regexp.
// { dg-regexp {[^\n]*p4405-constexpr\.json: In JSON value '/0/output/semantic'} }
// { dg-regexp {[^\n]*p4405-constexpr\.json:[0-9]+:[0-9]+: error: the 'semantic' of an entry matching 'constexpr' cannot be 'prove' or 'diagnose'[^\n]*} }
// { dg-regexp {[^\n]*p4405-constexpr\.json: In JSON value '/1/output/semantic'} }
// { dg-regexp {[^\n]*p4405-constexpr\.json:[0-9]+:[0-9]+: error: the 'semantic' of an entry matching 'constexpr' cannot be 'prove' or 'diagnose'[^\n]*} }

constexpr int f (int x) pre (x > 0) post (r: r > 0)
{
  contract_assert (x > 0);
  return x;
}

static_assert (f (-1) == -1);
constexpr int v = f (-2);
static_assert (v == -2);
