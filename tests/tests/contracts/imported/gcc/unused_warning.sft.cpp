//remark: imported from gcc:unused_warning.C
//type: fp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contract_evaluation_semantic=enforce
// check that we do not get unused warnings for contract check function parameters
// { dg-do compile { target c++23 } }
// { dg-additional-options "-fcontracts -fcontract-evaluation-semantic=enforce  -Wall -Wextra" }

struct TimeInterval{
  int i = 4;
  void addInterval(int i, int j);
};

TimeInterval operator-(const TimeInterval& lhs, const TimeInterval& rhs)
    pre(2 != rhs.i);

inline
TimeInterval operator-(const TimeInterval& lhs,
                                   const TimeInterval& rhs)

{
    TimeInterval result(lhs);
    result.addInterval(-rhs.i, -rhs.i);
    return result;
}

int main(int, char**) {

}
