//remark: imported from gcc:postcondition-nonconst-param-redecl.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts
//match_regex: ", line 13: (?:catastrophic )?error
// EDG: adapted -- the front end reports the parameter at its declaration (line 7 of the original).
// GCC-938: a postcondition diagnosed as odr-using a value parameter that is
// not const is erroneous, and is not compared with a redeclaration's
// ("mismatched contract condition").
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts" }

int g (int x)			// { dg-message "parameter declared here" }
  post (r: r > x);		// { dg-error "a value parameter used in a postcondition must be const" }

int g (const int x)
  post (r: r > x)		// { dg-bogus "mismatched" }
{
  return x + 1;
}
