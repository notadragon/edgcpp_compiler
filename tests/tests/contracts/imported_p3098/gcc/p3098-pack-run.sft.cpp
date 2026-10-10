//remark: imported from gcc:p3098-pack-run.C
//type: rp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3098 --contract_evaluation_semantic=enforce
//use_system_includes: true
//linker_options: -lcontracts
// P3098: Pack captures -- runtime verification.
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3098 -fcontract-evaluation-semantic=enforce" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

// Simple pack capture: captures hold call-time values.
template <typename... Args>
bool check_all_positive(Args... args)
{ return (... && (args > 0)); }

template <typename... Args>
bool f(Args... args)
  post [args...] (r: r && check_all_positive(args...))
{
  return true;
}

// Init-capture pack.
template <typename... Args>
bool g(Args... args)
  post [...old = args] (r: check_all_positive(old...))
{
  return true;
}

int main() {
  // Simple pack capture with 3 args.
  if (!f(1, 2, 3))
    __builtin_abort ();

  // Init-capture pack.
  if (!g(10, 20, 30))
    __builtin_abort ();

  // Single-element pack.
  if (!f(42))
    __builtin_abort ();
}
