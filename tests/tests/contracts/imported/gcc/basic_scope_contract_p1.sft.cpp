//remark: imported from gcc:basic.scope.contract.p1.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contract_evaluation_semantic=observe
//match_regex: ", line 22: (?:catastrophic )?error
//match_regex: ", line 27: (?:catastrophic )?error
//match_regex: ", line 36: (?:catastrophic )?error
//match_regex: ", line 45: (?:catastrophic )?error
// N5008
// basic.scope.contract/p1
// Each contract assertion (6.10) C introduces a contract-assertion scope that includes C.
// { dg-do compile { target c++23 } }
// { dg-additional-options "-fcontracts -fcontract-evaluation-semantic=observe " }

// Check that result vars are not visible in function bodies.

int free_fn (int x)
  post (r: r > 5);

int free_fn (int x)
  post (r: r > 5)
{ r += 1; return r;} // { dg-error {'r' was not declared in this scope} }

struct X {
  int fn0 (int x)
    post (res: res > 5)
    { res = 15; return res; } // { dg-error {'res' was not declared in this scope} }

  int fn1 (int x)
    post (res: res > 5);
};

int
X::fn1 (int y)
    post (ans: ans > 5)
    { ans = 15; return 1; } // { dg-error {'ans' was not declared in this scope} }

template <class T>
int
postcond (T x) post (res: res > 3); 

template <class T>
int
postcond (T x) post (out: out > 3)
{ out = 5;  return x; } // { dg-error {'out' was not declared in this scope} }

int foo (int x)
{
  return postcond (4);
}
