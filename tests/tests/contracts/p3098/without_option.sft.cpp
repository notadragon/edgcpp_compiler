//remark:contracts: without P3098 a postcondition cannot have a capture list
//type:fn
//options:--no_contracts_p3098:--contracts_p3850 --no_contracts_p3098
//match_regex:line 5: error: postcondition captures require --contracts_p3098
int f(int x) post [y = x] (true);
