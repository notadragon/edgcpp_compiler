# C++ Contracts Implementation (P3850) in the EDG front end

This fork of the EDG C/C++ front end is for implementing C++ Contracts
(P2900) and the extensions proposed in P3850.

Everything added here is prototype-quality, for implementation experience
and as a foundation for future upstream integration.

This branch is part of a larger project to implement Contracts features
across C++ compilers, and details on the current status and scope of these
changes can be found at https://github.com/notadragon/contracts-p3850.

## Compiler Explorer

A build of this branch is available on compiler explorer.  You can use it by
selecting the "EDG P3850 contracts (GNU mode, notadragon gcc)" entry in the
compiler dropdown when editing C++ code at https://godbolt.org/.

Current nightly build status:
[![Status](https://github.com/compiler-explorer/compiler-workflows/actions/workflows/build-daily-edg_notadragon_contracts_p3850.yml/badge.svg)](https://github.com/compiler-explorer/compiler-workflows/actions/workflows/build-daily-edg_notadragon_contracts_p3850.yml)![Last success](https://img.shields.io/badge/dynamic/json?color=success&label=Last+OK&query=%24.last_success.timestamp&url=https%3A%2F%2Flambda.compiler-explorer.com%2Fcompiler-build%2Fedg_notadragon_contracts_p3850)![Last build](https://img.shields.io/badge/dynamic/json?color=yellow&label=Last+build&query=%24.last_build.timestamp&url=https%3A%2F%2Flambda.compiler-explorer.com%2Fcompiler-build%2Fedg_notadragon_contracts_p3850)

## Known Bugs and Open Issues

Open issues with this branch are tracked in the branch itself so that
consumers have one place to go to see the current state.  Details can be
found in two distinct lists:

- [open-issues/README.md](open-issues/README.md) -- **everything
  contracts-related that is known to be broken on this branch**, whatever
  its origin.  If you encounter issues, double-check here to see if they
  are known.  Some of these are underlying issues that we have deferred,
  others are in progress.
- [bug-reports/README.md](bug-reports/README.md) -- bugs that exist
  upstream, some of which have been fixed in this branch.

Some items that exist upstream which we have not yet fixed are in both
lists.

## Contact

Joshua Berne -- jberne4@bloomberg.net

---

# EDG Compiler Project

Welcome to the open source EDG C/C++ compiler project!

## About

The EDG C/C++ front end is the primary focus of this repository.  The EDG front
end is known for: its excellent parsing compatibility and bug emulation, to
make sure source that compiles with Clang, GCC, and MSVC compiles with EDG;
extensive documentation, to assist with development and modification; and
extreme configurability, to allow it to be derived for use into a large number
of C/C++ oriented products.

The compiler project also includes: a C-generating back end, which can be used
to generate C code for C++ programs; a C++-generating back end, which is useful
for source-to-source transformation applications; a prelinker, which handles
automatic template instantiation; a minimal runtime support library (but not
any "real" libraries, e.g., for stream I/O); utilities to write the
intermediate language to a file, read it back in, and display it in
human-readable form; a name demangler; and a collection of purpose built
development tools.

## Documentation

See the documentation on: https://edgcpp.org/doc/.  Documentation for the
current development branch (`main`) is on: https://edgcpp.org/dev/doc/.

> [!NOTE]
>
> If you would like to build a local copy of the documentation see the
> [documentation build guide](doc/README.md).

## Building & Contributing

To get started, it's recommended for most people to clone the repository using
a partial clone (this keeps a full copy of the git history while only
downloading the most recent version of each file, git can implicitly fetch
older versions as needed):

```
git clone --filter=blob:none git@github.com:edgcpp/compiler.git
```

If you are looking to build the front end from source and use it, please
see [the build instructions](BUILD.md).

If you are looking to make changes to the front end, please see the
[contribution guide](CONTRIBUTING.md) and the [development guide](HACKING.md)
for quickly getting setup.

## Tips, Tricks, & Tutorials

To further aid humans and AI agents in product development there is additional
documentation for tips, tricks, and tutorials that can be found in the
[development annex](dev_annex/README.md).
