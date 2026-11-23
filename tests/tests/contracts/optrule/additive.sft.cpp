//remark:contracts: the contracts option rule (notadragon_wg21 DECISIONS.md E175), as our GCC's contract-options-additive-*: --contracts_p3850, --contracts_p3400 and --contracts_p4451 are additive, whatever their order: P3400 is on when given or implied by --contracts_p3850, P4451 when given or when P3400 or P3850 is on, and contracts when any of them is on (C++23, so that only the options turn contracts on); --no_contracts_p4451 with P3400 on is an error
//type:fp
//options:--c++23 --no_contracts_p3850 --no_contracts_p3400 --no_contracts_p4451:--c++23 --no_contracts_p3850 --no_contracts_p3400:--c++23 --no_contracts_p3850 --no_contracts_p3400 --contracts_p4451:--c++23 --no_contracts_p3850 --no_contracts_p4451:--c++23 --no_contracts_p3850:--c++23 --no_contracts_p3850 --contracts_p4451:--c++23 --no_contracts_p3850 --contracts_p3400 --no_contracts_p4451;fc:--c++23 --no_contracts_p3850 --contracts_p3400:--c++23 --no_contracts_p3850 --contracts_p3400 --contracts_p4451:--c++23 --no_contracts_p3400 --no_contracts_p4451:--c++23 --no_contracts_p3400:--c++23 --no_contracts_p3400 --contracts_p4451:--c++23 --no_contracts_p4451:--c++23:--c++23 --contracts_p4451:--c++23 --contracts_p3400 --no_contracts_p4451;fc:--c++23 --contracts_p3400:--c++23 --contracts_p3400 --contracts_p4451:--c++23 --contracts_p3850 --no_contracts_p3400 --no_contracts_p4451:--c++23 --contracts_p3850 --no_contracts_p3400:--c++23 --contracts_p3850 --no_contracts_p3400 --contracts_p4451:--c++23 --contracts_p3850 --no_contracts_p4451;fc:--c++23 --contracts_p3850:--c++23 --contracts_p3850 --contracts_p4451:--c++23 --contracts_p3850 --contracts_p3400 --no_contracts_p4451;fc:--c++23 --contracts_p3850 --contracts_p3400:--c++23 --contracts_p3850 --contracts_p3400 --contracts_p4451:--c++23 --contracts_p3400 --no_contracts_p3850:--c++23 --contracts_p4451 --no_contracts_p3400:--c++23 --no_contracts_p4451 --contracts_p3400;fc:--c++23 --no_contracts_p4451 --no_contracts_p3400 --contracts_p3850:--c++23 --no_contracts_p3400 --contracts_p3850:--c++23 --no_contracts_p4451 --contracts_p3850;fc
//match_regex:^(Command-line error: .*|)$
// Cases: every combination of --contracts_p3850, --contracts_p3400 and
// --contracts_p4451 given (y), given as --no_ (n) or not given (x), in that
// order, then six in the reverse order (GCC's *-rev tests).  P3097, which
// --contracts_p3850 implies, raises __cpp_contracts too, so P4451 being off
// is seen only through __gcc_contracts_p4451, in GNU mode (edg_x86_64_gxx).
//
// Mirror: gcc/testsuite/g++.dg/contracts/cpp26/contract-options-additive-*.C
#if TEST_NUMBER == 3 || TEST_NUMBER == 6 || TEST_NUMBER == 8 || \
    TEST_NUMBER == 9 || TEST_NUMBER == 12 || TEST_NUMBER == 15 || \
    TEST_NUMBER == 17 || TEST_NUMBER == 18 || TEST_NUMBER == 19 || \
    TEST_NUMBER == 20 || TEST_NUMBER == 21 || TEST_NUMBER == 23 || \
    TEST_NUMBER == 24 || TEST_NUMBER == 26 || TEST_NUMBER == 27 || \
    TEST_NUMBER == 28 || TEST_NUMBER == 29 || TEST_NUMBER == 31 || \
    TEST_NUMBER == 32
#define CONTRACTS 1
#endif
#if TEST_NUMBER == 8 || TEST_NUMBER == 9 || TEST_NUMBER == 17 || \
    TEST_NUMBER == 18 || TEST_NUMBER == 23 || TEST_NUMBER == 24 || \
    TEST_NUMBER == 26 || TEST_NUMBER == 27 || TEST_NUMBER == 28
#define P3400 1
#endif
#if TEST_NUMBER == 3 || TEST_NUMBER == 6 || TEST_NUMBER == 8 || \
    TEST_NUMBER == 9 || TEST_NUMBER == 12 || TEST_NUMBER == 15 || \
    TEST_NUMBER == 17 || TEST_NUMBER == 18 || TEST_NUMBER == 20 || \
    TEST_NUMBER == 21 || TEST_NUMBER == 23 || TEST_NUMBER == 24 || \
    TEST_NUMBER == 26 || TEST_NUMBER == 27 || TEST_NUMBER == 28 || \
    TEST_NUMBER == 29 || TEST_NUMBER == 32
#define P4451 1
#endif
#if defined(CONTRACTS) != defined(__cpp_contracts)
#error __cpp_contracts
#endif
#if defined(P3400) != defined(__cpp_contracts_labels)
#error __cpp_contracts_labels
#endif
#if defined(__GNUC__) && defined(P4451) != defined(__gcc_contracts_p4451)
#error __gcc_contracts_p4451
#endif
#if defined(P4451) && !(__cpp_contracts > 202502L)
#error __cpp_contracts not raised by P4451
#endif
int i;
