//remark: imported from gcc:p3400-rethrow-bypass-weak-callee-2.cc
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26
// Second translation unit for p3400-rethrow-bypass-weak-callee.C:
// the strong definition that replaces the weak rethrow_helper at link time.

extern int logged;
[[noreturn]] void rethrow_helper () { ++logged; throw; }
