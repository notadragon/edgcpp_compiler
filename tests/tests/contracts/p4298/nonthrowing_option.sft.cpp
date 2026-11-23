//remark:contracts: without P4298 a noexcept semantic is replaced by its throwing variant with a warning, as GCC does, and __cpp_contracts_nonthrowing_semantics is not defined
//require:BACK_END_IS_CP_GEN_BE 1
//type:fp
//options:--contract_evaluation_semantic=noexcept_enforce -DWANT=0:--contract_evaluation_semantic=noexcept_observe -DWANT=0:--contracts_p4298 --contract_evaluation_semantic=noexcept_enforce -DWANT=1
//match_regex:^(Command-line warning: --contract_evaluation_semantic=noexcept_(enforce|observe) requires --contracts_p4298; (enforce|observe) is used instead|)$
#if defined(__cpp_contracts_nonthrowing_semantics) != WANT
#error the macro
#endif
#if WANT && __cpp_contracts_nonthrowing_semantics <= 0
#error the value
#endif
int x;
