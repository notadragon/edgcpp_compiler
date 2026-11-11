//remark: imported from gcc:pr125904.C
//type: rp
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contract_evaluation_semantic=observe
//use_system_includes: true
//linker_options: -lcontracts
// { dg-do run { target c++26 } }
// { dg-additional-options "-fcontracts -fcontract-evaluation-semantic=observe" }
// { dg-skip-if "requires hosted libstdc++ for stdc++exp" { ! hostedlib } }

#include <iostream>
#include <contracts>

#define VERIFY_ASSERT(statement, asserts)  \
	{ \
		bool violation = false;\
		try{ \
			statement; \
		} catch(int &ex) { \
			violation = true; \
		} \
		if ((asserts && !violation) || (!(asserts) && violation)) __builtin_abort(); \
	} \

static_assert (__cpp_contracts >= 202502L);

void handle_contract_violation(const std::contracts::contract_violation &violation) {
  std::cerr << "custom std::handle_contract_violation called:"
    << " " << violation.location().line()
    << " " << violation.location().file_name()
    << std::endl;
  throw -(int)violation.location().line();
}

void f1()
{
  for (int i = 0; i < 10; ++i)
    contract_assert(false);
}

void f2()
{
  do {
    contract_assert(false);
  } while (false);
}

void f3()
{
  int i = 1;
  while (i--)
    contract_assert(false);
}

int main()
{
  VERIFY_ASSERT(f1(), true);
  VERIFY_ASSERT(f2(), true);
  VERIFY_ASSERT(f3(), true);
  return 0;
}
