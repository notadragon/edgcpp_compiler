//remark: imported from gcc:p4405-analysis-flag-invalid.C
//type: fc
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p4405 --contract_analysis_semantic=enforce
//match_regex: ^(Command-line error: .*)$
// EDG: adapted -- the front end rejects --contract_analysis_semantic=enforce
// as a command-line error (type fc), before g++ sees the option.
// D4405: the analysis evaluation's semantic is one of ignore, diagnose and
// prove; -fcontract-analysis-semantic= rejects any other.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p4405 -fcontract-analysis-semantic=enforce" }
// { dg-error "the analysis evaluation semantic must be 'ignore', 'diagnose' or 'prove', not 'enforce'" "" { target *-*-* } 0 }

int f (int x) pre (x > 0) { return x; }
