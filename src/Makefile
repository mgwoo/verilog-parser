EXE = parser

CXX = g++
BISON = bison
FLEX = ../module/flex/install/2.6.4/bin/flex


CXXOPTS = -m64 -O3 -fPIC -fno-strict-aliasing -fexceptions -DIL_STD -Wno-ctor-dtor-privacy -fno-permissive
CFLAGS =  -O3 -fPIC -fno-permissive

PREFIX = ..

# linking flag settings
LINKCFLAGS = $(CFLAGS) -static-libstdc++
 
DEBUG = -g -gstrict-dwarf -gdwarf-2

.PHONY: all
all: $(EXE) static

$(EXE): verilog_ast.o verilog_ast_common.o verilog_ast_mem.o verilog_ast_util.o verilog_parser.tab.o verilog_parser_wrapper.o verilog_preprocessor.o verilog_scanner.o verilog_writer.o main.o
	$(CXX) $(CXXOPTS) $^ -o $@ $(LINKCFLAGS)

# Verilog Traversing Module
verilog_writer.o: verilog_writer.cpp verilog_writer.h verilog_parser.h
	$(CXX) $(CFLAGS) -c -o $@ $<

# Verilog Parsing Module
verilog_scanner.c: verilog_scanner.l verilog_parser.tab.c verilog_global.h
	$(FLEX) $<
	mv lex.yy.c verilog_scanner.c

verilog_parser.tab.c: verilog_parser.y verilog_global.h
	$(BISON) $<

verilog_ast.o: verilog_ast.c verilog_ast.h verilog_preprocessor.h verilog_global.h
	$(CXX) $(CFLAGS) -c -o $@ $<

verilog_ast_common.o: verilog_ast_common.c verilog_ast_common.h verilog_global.h
	$(CXX) $(CFLAGS) -c -o $@ $<

verilog_ast_mem.o: verilog_ast_mem.c verilog_ast_mem.h verilog_global.h
	$(CXX) $(CFLAGS) -c -o $@ $<

verilog_ast_util.o: verilog_ast_util.c verilog_ast_util.h verilog_ast.h verilog_global.h
	$(CXX) $(CFLAGS) -c -o $@ $<

verilog_parser.tab.o: verilog_parser.tab.c verilog_ast.h verilog_global.h
	$(CXX) $(CFLAGS) -c -o $@ $<

verilog_parser_wrapper.o: verilog_parser_wrapper.c verilog_ast.h verilog_parser.h verilog_global.h
	$(CXX) $(CFLAGS) -c -o $@ $<

verilog_preprocessor.o: verilog_preprocessor.c verilog_preprocessor.h verilog_ast.h verilog_ast_common.h verilog_global.h
	$(CXX) $(CFLAGS) -c -o $@ $<

verilog_scanner.o: verilog_scanner.c verilog_ast.h verilog_preprocessor.h verilog_global.h
	$(CXX) $(CFLAGS) -c -o $@ $<

main.o: main.cpp verilog_parser.h verilog_ast_common.h verilog_preprocessor.h verilog_ast_util.h verilog_global.h 
	$(CXX) $(CFLAGS) -c -o $@ $<

.PHONY: static
static: libverilog_parser.a

libverilog_parser.a: verilog_ast.o verilog_ast_common.o verilog_ast_mem.o verilog_ast_util.o verilog_parser.tab.o verilog_parser_wrapper.o verilog_preprocessor.o verilog_scanner.o verilog_writer.o
	ar rc $@ $^
	mkdir -p $(PREFIX)/lib
	mv $@ $(PREFIX)/lib/$@
	mkdir -p $(PREFIX)/include
	cp *.h $(PREFIX)/include


.PHONY: clean
clean: 
	/bin/rm -f *.o $(EXE) verilog_parser.tab.c verilog_parser.tab.h verilog_scanner.c verilog_parser.output $(PREFIX)/lib/libverilog_parser.a $(PREFIX)/include/*.h parser 2> /dev/null;


