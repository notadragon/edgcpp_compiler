//remark:contracts: EDG-117: a P3098 init-capture with its ellipsis after the name is diagnosed as a capture without an initializer, and the capture is dropped
//require:BACK_END_IS_CP_GEN_BE 1
//type:fn
//options:--contracts_p3098
//match_regex:", line 15: error
//match_regex:", line 17: error
//match_regex:", line 19: error
// The recording is the current, wrong behavior: when this test deviates,
// the bug may be fixed.  Correct: one error on each line, saying that the
// ellipsis of a pack init-capture goes before the name ('...y ='), and no
// other (the capture recovers as the pack '...y = xs').  GCC (GCC-681) and
// Clang (CLANG-663) do that.  From GCC's p3098-unexpanded-pack-capture.C
// and Clang's p3098-unexpanded-pack-capture.cpp (EDG-115, EDG-116).
template <class... T>
int bad3(T... xs) post [y... = xs] (((y > 0) && ...)) { return 0; }
template <class... T>
int bad4(T... xs) post [...y... = xs] (((y > 0) && ...)) { return 0; }
template <class... T>
int d7(T... xs) post [y... = xs, z = 0] (((y > z) && ...));
