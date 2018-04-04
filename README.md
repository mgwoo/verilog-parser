
# C++ Verilog Parser/Writer

[![Documentation](https://codedocs.xyz/ben-marshall/verilog-parser.svg)](https://codedocs.xyz/ben-marshall/verilog-parser/)
[![Build Status](https://travis-ci.org/ben-marshall/verilog-parser.svg?branch=master)](https://travis-ci.org/ben-marshall/verilog-parser/branches)
[![Coverage Status](https://coveralls.io/repos/github/ben-marshall/verilog-parser/badge.svg?branch=master)](https://coveralls.io/github/ben-marshall/verilog-parser?branch=master)
![Licence: MIT](https://img.shields.io/badge/License-MIT-blue.svg)

This repository was forked from [ben-marshall's verilog parser in C](https://github.com/ben-marshall/verilog-parser.git). This project is target to change C into modern C++ (i.e. modern g++ successfully can compile this project)

- [Getting Started](#getting-started)
- [Testing](#testing)
- [Solved Issue](#solved-issue)

---

## Getting Started

This will get you have c++ verilog parsing binary.

    $ cd src/
    $ make clean
    $ make -j4

To start using the parser in your own code, take a look at 
[main.cpp](./src/main.cpp) which is a simple demonstration app used for testing
and coverage. The basic code which you need is something like this:

```C++
// Initialise the parser.
verilog_parser_init();

// Open A File handle to read data in.
FILE * fh = fopen("my_verilog_file.v", "r");

// Parse the file and store the result.
int result = verilog_parse_file(fh);

verilog::verilog_source_tree* ast = verilog::yy_verilog_source_tree;
verilog::verilog_resolve_modules(ast);

FILE* fout = fopen("writing_veilog.v", "w");
PrintVerilog(fout, ast);


```

You can keep calling `verilog_parse_file(fh)` on as many different file
handles as you like to build up a multi-file project AST representation.
The parser will automatically follow any `include` directives it finds.

## Testing

The original author used [ASIC World](http://www.asic-world.com/) 
and [OpenSPARCT1](http://www.oracle.com/technetwork/systems/opensparc/opensparc-t1-page-1444609.html)
to test codes, but I mainly focused on benchmarks of 
[ICCAD Contest 2015; Problem C](http://cad-contest.el.cycu.edu.tw/problem_C/default.html), 
kinds of gate-level netlist. 


---

## Solved issue

- Both of gcc/g++ now successfully compile this project (void ptr/extern variable/operator problem)
- Added static library linking options. (libverilog\_parser.a)
- Apply namespace(verilog) as not to conflict with oter variable name.
- Implement Verilog Writer (with custom line break)

