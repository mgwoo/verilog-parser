
# C++ Verilog Parser

[![Documentation](https://codedocs.xyz/ben-marshall/verilog-parser.svg)](https://codedocs.xyz/ben-marshall/verilog-parser/)
[![Build Status](https://travis-ci.org/ben-marshall/verilog-parser.svg?branch=master)](https://travis-ci.org/ben-marshall/verilog-parser/branches)
[![Coverage Status](https://coveralls.io/repos/github/ben-marshall/verilog-parser/badge.svg?branch=master)](https://coveralls.io/github/ben-marshall/verilog-parser?branch=master)
![Licence: MIT](https://img.shields.io/badge/License-MIT-blue.svg)

This repository was forked from [ben-marshall's verilog parser in C](https://github.com/ben-marshall/verilog-parser.git). This project is target to change C into modern C++ (i.e. modern g++ successfully can compile this project)

- [Getting Started](#getting-started)
- [Testing](#testing)
- [Solved Issue](#solved-issue)
- [Todo List](#todo)

---

## Getting Started

This will get you have c++ verilog parsing binary.

    $ cd src/
    $ make clean
    $ make -j4

To start using the parser in your own code, take a look at 
[main.c](./src/main.c) which is a simple demonstration app used for testing
and coverage. The basic code which you need is something like this:

```C
// Initialise the parser.
verilog_parser_init();

// Open A File handle to read data in.
FILE * fh = fopen("my_verilog_file.v", "r");

// Parse the file and store the result.
int result = verilog_parse_file(fh);

if(result == 0)
    printf("Parse successful\n");
else
    printf("Parse failed\n");

fclose(fh);
```

You can keep calling `verilog_parse_file(fh)` on as many different file
handles as you like to build up a multi-file project AST representation.
The parser will automatically follow any `include` directives it finds.

For an example of using the library in a real*ish* situation, the
[verilog-dot](https://github.com/ben-marshall/verilog-dot) project shows how
the library can be integrated into an existing project and used.

## Testing

The test suite is comprised of example code taken from
the fantastic [ASIC World](http://www.asic-world.com/) tutorial on Verilog.
The idea being that by using a well-known and comprehensive set of
tutorial examples, almost all of the syntactic features of the language can be
hit very easily with little effort.

The repository also contains an archive of verilog source code taken from the
[OpenSPARCT1](http://www.oracle.com/technetwork/systems/opensparc/opensparc-t1-page-1444609.html)
microprocessor from Oracle. This archive is unpacked into the `tests/`
directory when `make setup` is run, and ensures that the parser is able to
handle a *real-life* source base in terms of scale and complexity.  The full
workspace environment required to run or analyse the OpenSPARCT1 is not
provided, the files only demonstrate the ability to correctly parse a large
project, and handle the various internal header files and preprocessor
definitions.

---

## Solved issue

- Both of gcc/g++ now successfully compile this project (void ptr/extern variable/operator problem)
- Added static library linking options. (libverilog\_parser.a)
 
## Todo

- Implement Verilog Writer(with custom indent)
- Multiple module parsing support


