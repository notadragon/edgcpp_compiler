//remark: imported from gcc:p4451-std-cxx26-no-p4451-p3400.C
//type: fc
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --no_contracts_p4451 --contracts_p3400
//match_regex: ^(Command-line error: --no_contracts_p4451 cannot be combined with --contracts_p3400, which subsumes P4451)$
// EDG: adapted -- the front end rejects the options as a command-line error
// (type fc), before g++ sees them.
// C++26 enables -fcontracts-p4451, and an explicit -fno-contracts-p4451
// turns it off under the usual option rule: with -fcontracts-p3400, which
// subsumes P4451, that is still an error.
// { dg-do compile }
// { dg-options "-std=c++26 -fno-contracts-p4451 -fcontracts-p3400" }
// { dg-error ".-fno-contracts-p4451. cannot be combined with .-fcontracts-p3400., which subsumes P4451" "" { target *-*-* } 0 }

int i;
