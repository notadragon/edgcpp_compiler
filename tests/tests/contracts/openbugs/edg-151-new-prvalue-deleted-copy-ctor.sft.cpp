//remark:contracts: EDG-151 (stock): cpfe-cp selects the copy constructor for a new-expression initialized from a prvalue of the same class, so a deleted one is an error
//require:BACK_END_IS_CP_GEN_BE 1
//require:NEW_CAN_BE_FOLDED_INTO_CTOR 1
//type:fn
//match_regex:line ([0-9]+): error: (.*)
// Records the current (wrong) behavior: both new-expressions are valid
// (C++17 initializes the object directly from the prvalue), and the
// C-generating front end accepts them, as does the C++-generating one in
// the Debug configuration, which does not fold the allocation into the
// constructor call.  A fix shows up as a deviation.
struct T {
  T();
  T(const T &) = delete;
};
T make();
T *a = new T(T());
T *b = new T(make());
T *c = new T{T()};
