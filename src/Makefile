EXE = parser

CXX = g++
BISON = bison
FLEX = flex

CXXOPTS = -m64 -O3 -fPIC -mtune=native -march=native -fno-strict-aliasing -fexceptions -DNDEBUG -DIL_STD -Wno-ctor-dtor-privacy -fno-permissive

CFLAGS =  -O3 -fPIC -mtune=native -march=native -g -ggdb -DNDEBUG -fno-permissive


# linking flag settings
LINKCFLAGS = $(CFLAGS) -static-libstdc++
 
DEBUG = -g -gstrict-dwarf -gdwarf-2

.PHONY: all
all: $(EXE) static

$(EXE): verilog_ast.o verilog_ast_common.o verilog_ast_mem.o verilog_ast_util.o verilog_parser.tab.o verilog_parser_wrapper.o verilog_preprocessor.o verilog_scanner.o main.o
	$(CXX) $(DEBUG) $(CXXOPTS) $^ -o $@ $(LINKCFLAGS)

verilog_scanner.c: verilog_scanner.l verilog_parser.tab.c
	$(FLEX) $<
	mv lex.yy.c verilog_scanner.c

verilog_parser.tab.c: verilog_parser.y
	$(BISON) $<

verilog_ast.o: verilog_ast.c verilog_ast.h verilog_preprocessor.h
	$(CXX) $(CFLAGS) -c -o $@ $<

verilog_ast_common.o: verilog_ast_common.c verilog_ast_common.h
	$(CXX) $(CFLAGS) -c -o $@ $<

verilog_ast_mem.o: verilog_ast_mem.c verilog_ast_mem.h
	$(CXX) $(CFLAGS) -c -o $@ $<

verilog_ast_util.o: verilog_ast_util.c verilog_ast_util.h verilog_ast.h
	$(CXX) $(CFLAGS) -c -o $@ $<

verilog_parser.tab.o: verilog_parser.tab.c verilog_ast.h
	$(CXX) $(CFLAGS) -c -o $@ $<

verilog_parser_wrapper.o: verilog_parser_wrapper.c verilog_ast.h verilog_parser.h
	$(CXX) $(CFLAGS) -c -o $@ $<

verilog_preprocessor.o: verilog_preprocessor.c verilog_preprocessor.h verilog_ast.h verilog_ast_common.h
	$(CXX) $(CFLAGS) -c -o $@ $<

verilog_scanner.o: verilog_scanner.c verilog_ast.h verilog_preprocessor.h
	$(CXX) $(CFLAGS) -c -o $@ $<

main.o: main.c verilog_parser.h verilog_ast_common.h verilog_preprocessor.h verilog_ast_util.h
	$(CXX) $(CFLAGS) -c -o $@ $<

.PHONY: static
static: libverilog_parser.a

libverilog_parser.a: verilog_ast.o verilog_ast_common.o verilog_ast_mem.o verilog_ast_util.o verilog_parser.tab.o verilog_parser_wrapper.o verilog_preprocessor.o verilog_scanner.o 
	ar rc $@ $^ 


.PHONY: clean
clean: 
	/bin/rm -f *.o $(EXE) verilog_parser.tab.c verilog_parser.tab.h verilog_scanner.c verilog_parser.output libverilog_parser.a parser 2> /dev/null;


