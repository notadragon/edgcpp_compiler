//remark: imported from gcc:dependent_contract.C
//type: rp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contract_evaluation_semantic=observe
//use_system_includes: true
//linker_options: -lcontracts
// check that dependent contract check does not cause an ICE
// { dg-do run { target c++23 } }
// { dg-additional-options "-fcontracts -fcontract-evaluation-semantic=observe " }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }
template <typename ST>
struct S{

  int* check() const
  {
      static int i = 6;
      return &i;
  }

  template <typename T>
  T* tcheck(T) const{
    static T t;
    return nullptr;
  }

  void f() pre(check()) pre(tcheck(1)){
  }

  template <typename T>
  void f(T t) pre(check()) pre(tcheck(1)) pre(tcheck<T>(t)){
  }

};

int main() {
    S<int> s;
    s.f();
    s.f(1);
}
