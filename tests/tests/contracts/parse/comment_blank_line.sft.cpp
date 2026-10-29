//remark:contracts: the comment of a contract whose predicate spans lines holding only blanks is its tokens on one line (no crash)
//require:DO_IL_LOWERING 1
//type:fp
//options_all:--no_il_lowering --il_display --contract_evaluation_semantic=ignore
//filter:awk '/^kind: +ctk_/ { k = $2 } /^comment: / { sub(/^comment: +[^ ]+ other-text@[^ ]+ /, ""); print k, $0 }'
// Mirror of our GCC's g++.dg/contracts/cpp26/contract-comment-blank-line.C
// (the gnu_gcc fork's GCC-706: g++ ICEd on a line of only blanks) and of
// Clang's clang/test/Contracts/contract-comment-blank-line.cpp, with the
// same shapes.  The front end forms the comment from the predicate's tokens
// (scan_contract_predicate), separated by one space, so line breaks and
// blank lines leave no trace.  The comment is implementation-defined; this
// pins the front end's own, seen in the IL (C-generating back end only:
// through edg-gxx every comment is g++'s printed expression, see
// comment_gxx).  The filter prints each contract's kind and comment, in
// the order of the IL display (a class's member functions come first).

// A line of two spaces.
int pre_spaces(int x) pre(x
  
  > 0) { return x; }

// A line of a tab.
int pre_tab(int x) pre(x
	
	> 0) { return x; }

// Spaces and a tab, and two blank lines in a row.
int pre_mixed(int x) pre(x
 	 
  
  > 0) { return x; }

// An empty line.
int pre_empty(int x) pre(x

  > 0) { return x; }

// A continuation line that is not indented.
int pre_flush(int x) pre(x
> 0) { return x; }

int post_spaces(int x) post(r: r
  
  > 0) { return x; }

// A member function's contracts are parsed at the end of the class.
struct S {
  int member(int x) pre(x
  
    > 0) { return x; }
};

void f() {
  S().member(-1);  // Used, or the IL drops the unneeded member function.
  int x = -1;
  contract_assert(x < 0
  
                  && x > 0);
}
