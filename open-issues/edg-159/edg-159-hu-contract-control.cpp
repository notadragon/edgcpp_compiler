// Importer for edg-159-hu-contract-control.h (--contracts_p3400).
// Expected (and given when the header is #included instead): errors at
// leak1 and leak2 only.  Through the header unit: leak1 and leak2 are
// accepted, and the last line is an error.
import "edg-159-hu-contract-control.h";

// Active: an assertion-control expression.
void f1 (int x) pre<my_label_t{}> (x > 0);
void f2 (int x) pre<make_label()> (x > 0);

// Inactive: ordinary lookup.
auto leak1 = my_label_t{};  // Error
auto leak2 = make_label();  // Error

auto ok = g10ns::make_label();  // OK
const g10ns::my_label_t *p = &g10ns::my_label;  // OK
