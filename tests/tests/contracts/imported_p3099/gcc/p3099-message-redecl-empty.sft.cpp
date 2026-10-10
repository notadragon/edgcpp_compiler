//remark: imported from gcc:p3099-message-redecl-empty.C
//type: fn
//require: BACK_END_IS_CP_GEN_BE 1
//options: --c++26 --contracts --contracts_p3099
//match_regex: ", line 28: (?:catastrophic )?error
//match_regex: ", line 31: (?:catastrophic )?error
// EDG: adapted -- the user-defined message object shapes (h and j) are left
// out: computed messages are not supported (EDG-72, deferred).
// P3099: an empty diagnostic message is a message, distinct from none
// (P3099R3 sec. "Empty string vs. no string": message() is "" for the one
// and null for the other).  So a redeclaration that adds or drops an empty
// message is mismatched, in either order, while two empty messages match --
// including an empty string literal and a user-defined message object whose
// extracted text is empty, since sameness is decided on the extracted text.
//
// Mirror: clang/test/Contracts/p3099-message-redecl-empty.cpp in the
// llvm_llvm-project fork; owed to EDG, recorded in its open-issues/README.md.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3099" }

struct Empty
{
  constexpr int size () const { return 0; }
  constexpr const char *data () const { return ""; }
};

void f (int x) pre (x > 0, "");
void f (int x) pre (x > 0);		// { dg-error "mismatched contract diagnostic message" }

void g (int x) pre (x > 0);
void g (int x) pre (x > 0, "");		// { dg-error "mismatched contract diagnostic message" }


void i (int x) pre (x > 0, "");
void i (int x) pre (x > 0, "");		// OK

