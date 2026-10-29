//remark:contracts: P3098: a pack init-capture written with the ellipsis after the name ("y... = xs") is one error, recovered as "...y = xs" (EDG-117)
//require:BACK_END_IS_CP_GEN_BE 1
//type:fn
//options:--contracts_p3098
//match_regex:line 13: error: a pack init-capture is written "...y =", not "y... ="
//match_regex:line 15: error: a pack init-capture is written "...y =", not "y... ="
//match_regex:line 17: error: a pack init-capture is written "...y =", not "y... ="
// The ellipsis of a pack init-capture goes before the name (D3098R3's
// grammar, as in a lambda).  Each capture gets one error, and the predicate,
// which uses the recovered pack, nothing more; GCC (GCC-681) and Clang
// (CLANG-663) do the same.
template <class... T>
int bad3(T... xs) post [y... = xs] (((y > 0) && ...)) { return 0; }
template <class... T>
int bad4(T... xs) post [...y... = xs] (((y > 0) && ...)) { return 0; }
template <class... T>
int d7(T... xs) post [y... = xs, z = 0] (((y > z) && ...));
int main() { return bad3(1, 2) + bad4(1, 2, 3); }
