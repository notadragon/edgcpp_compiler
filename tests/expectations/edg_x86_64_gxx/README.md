Expectations for the edg_x86_64_gxx configuration (see .edgy/config.json).
A suite with no <suite>.log here is expected to have no problematic tests.

This is the contracts suite's default configuration
(tests/tests/contracts/.run_test_env), so every other configuration inherits
these lines (doc/source/testing.rst, "Expectation Inheritance").  A test
pinned to the C-generating back end is REQUIREMENTS NOT MET here and passes
under edg_x86_64, so tests/expectations/edg_x86_64/contracts.log holds an
explicit PASS line for it; edgy -W writes those lines itself.
