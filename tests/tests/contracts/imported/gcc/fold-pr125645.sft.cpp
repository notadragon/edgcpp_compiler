//remark: imported from gcc:fold-pr125645.C
//type: rp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contract_evaluation_semantic=enforce
//use_system_includes: true
//linker_options: -lcontracts
// PR c++/125645
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts -fcontract-evaluation-semantic=enforce" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

template<typename... Args>
  void f (Args... args)
    pre (((args) && ...))
    post (((args) && ...)) {}

int main () {
  f<const bool> (true);
}
