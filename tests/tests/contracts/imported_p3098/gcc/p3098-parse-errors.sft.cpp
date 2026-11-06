//remark: imported from gcc:p3098-parse-errors.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3098
//match_regex: ", line 20: (?:catastrophic )?error
//match_regex: ", line 23: (?:catastrophic )?error
//match_regex: ", line 26: (?:catastrophic )?error
//match_regex: ", line 29: (?:catastrophic )?error
//match_regex: ", line 33: (?:catastrophic )?error
//match_regex: ", line 35: (?:catastrophic )?error
//match_regex: ", line 40: (?:catastrophic )?error
//match_regex: ", line 43: (?:catastrophic )?error
//match_regex: ", line 46: (?:catastrophic )?error
// P3098: Postcondition captures -- parsing errors.
// Verifies all ill-formed capture forms are rejected.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3098" }

void f1(int i)
  post [&i] (true);       // { dg-error "capture-by-reference not allowed" }

void f2(int i)
  post [&i = i] (true);   // { dg-error "capture-by-reference not allowed" }

void f3(int i)
  post [=] (true);         // { dg-error "default capture not allowed" }

void f4(int i)
  post [&] (true);         // { dg-error "default capture not allowed" }

struct S {
  void f5()
    post [this] (true);    // { dg-error "cannot capture" }
  void f6()
    post [*this] (true);   // { dg-error "cannot capture" }
};

int global_var = 0;
void f7()
  post [global_var] (true);    // { dg-error "only function parameters can be captured" }

void f8(int i)
  pre [i] (i > 0);        // { dg-error "only allowed on" }

void f9(int i) {
  contract_assert [i] (i > 0); // { dg-error "only allowed on" }
}
