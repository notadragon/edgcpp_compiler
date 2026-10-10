//remark: imported from clang:p3098-pack-basic.cpp
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3098
// RUN: %clang_cc1 -std=c++26 -fcontracts -fcontracts-p3098 -fsyntax-only -verify %s
// expected-no-diagnostics

template<typename... Args>
int sum(Args... args) post [args...] (true) { return (args + ...); }

template int sum<int, int>(int, int);
template int sum<int, int, int>(int, int, int);
