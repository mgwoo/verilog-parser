/*!
@file main.c
@brief Contains the main entry point of the program.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "verilog_writer.h"
#include "verilog_parser.h"
#include "verilog_ast_util.h"

NAMESPACE_VERILOG_USING

int main(int argc, char ** argv) {
    
    FILE* input_file = fopen("ORCA_TOP.v", "rb");
    if( !input_file ) {
        printf("ERROR: Unable to open output file for writing:\n");
        exit(1);
    }

    // Initialise the parser.
    verilog_parser_init();
    int result = verilog_parse_file(input_file);

    verilog_source_tree * ast = yy_verilog_source_tree;

    // Resolve all of the names in the syntax tree.
    verilog_resolve_modules(ast);

    PrintVerilog(stdout, ast);
    printf("Writing complete!\n");
    
    return 0;
}
