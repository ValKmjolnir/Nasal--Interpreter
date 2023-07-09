STD=c++17

NASAL_HEADER=\
	src/ast_dumper.h\
	src/ast_visitor.h\
	src/nasal_ast.h\
	src/nasal_builtin.h\
	src/nasal_codegen.h\
	src/nasal_dbg.h\
	src/nasal_err.h\
	src/nasal_gc.h\
	src/nasal_import.h\
	src/nasal_lexer.h\
	src/nasal_opcode.h\
	src/nasal_parse.h\
	src/nasal_vm.h\
	src/nasal.h\
	src/optimizer.h\
	src/symbol_finder.h

NASAL_OBJECT=\
	build/nasal_err.o\
	build/nasal_ast.o\
	build/ast_visitor.o\
	build/ast_dumper.o\
	build/nasal_lexer.o\
	build/nasal_parse.o\
	build/nasal_import.o\
	build/optimizer.o\
	build/nasal_opcode.o\
	build/symbol_finder.o\
	build/nasal_codegen.o\
	build/nasal_misc.o\
	build/nasal_gc.o\
	build/nasal_builtin.o\
	build/nasal_vm.o\
	build/nasal_dbg.o\
	build/main.o

# for test
nasal: $(NASAL_OBJECT) | build
	$(CXX) $(NASAL_OBJECT) -O3 -o nasal -ldl

nasal.exe: $(NASAL_OBJECT) | build
	$(CXX) $(NASAL_OBJECT) -O3 -o nasal.exe

build:
	@ if [ ! -d build ]; then mkdir build; fi

build/main.o: $(NASAL_HEADER) src/main.cpp | build
	$(CXX) -std=$(STD) -c -O3 src/main.cpp -fno-exceptions -fPIC -o build/main.o -I .

build/nasal_misc.o: src/nasal.h src/nasal_misc.cpp | build
	$(CXX) -std=$(STD) -c -O3 src/nasal_misc.cpp -fno-exceptions -fPIC -o build/nasal_misc.o -I .

build/nasal_err.o: src/nasal.h src/nasal_err.h src/nasal_err.cpp | build
	$(CXX) -std=$(STD) -c -O3 src/nasal_err.cpp -fno-exceptions -fPIC -o build/nasal_err.o -I .

build/nasal_gc.o: src/nasal.h src/nasal_gc.h src/nasal_gc.cpp | build
	$(CXX) -std=$(STD) -c -O3 src/nasal_gc.cpp -fno-exceptions -fPIC -o build/nasal_gc.o -I . 

build/nasal_import.o: \
	src/nasal.h\
	src/nasal_ast.h\
	src/nasal_lexer.h\
	src/nasal_parse.h\
	src/nasal_import.h src/nasal_import.cpp | build
	$(CXX) -std=$(STD) -c -O3 src/nasal_import.cpp -fno-exceptions -fPIC -o build/nasal_import.o -I .

build/nasal_lexer.o: \
	src/nasal.h\
	src/nasal_err.h\
	src/nasal_lexer.h src/nasal_lexer.cpp | build
	$(CXX) -std=$(STD) -c -O3 src/nasal_lexer.cpp -fno-exceptions -fPIC -o build/nasal_lexer.o -I .

build/nasal_ast.o: \
	src/nasal.h\
	src/nasal_err.h\
	src/nasal_ast.h src/nasal_ast.cpp | build
	$(CXX) -std=$(STD) -c -O3 src/nasal_ast.cpp -fno-exceptions -fPIC -o build/nasal_ast.o -I .

build/nasal_builtin.o: \
	src/nasal.h\
	src/nasal_gc.h\
	src/nasal_builtin.h src/nasal_builtin.cpp | build
	$(CXX) -std=$(STD) -c -O3 src/nasal_builtin.cpp -fno-exceptions -fPIC -o build/nasal_builtin.o -I .

build/nasal_codegen.o: $(NASAL_HEADER) src/nasal_codegen.h src/nasal_codegen.cpp | build
	$(CXX) -std=$(STD) -c -O3 src/nasal_codegen.cpp -fno-exceptions -fPIC -o build/nasal_codegen.o -I .

build/nasal_opcode.o: \
	src/nasal.h\
	src/nasal_builtin.h\
	src/nasal_opcode.h src/nasal_opcode.cpp | build
	$(CXX) -std=$(STD) -c -O3 src/nasal_opcode.cpp -fno-exceptions -fPIC -o build/nasal_opcode.o -I .

build/nasal_parse.o: \
	src/nasal.h\
	src/nasal_ast.h\
	src/nasal_lexer.h\
	src/nasal_err.h\
	src/nasal_parse.h src/nasal_parse.cpp src/nasal_ast.h | build
	$(CXX) -std=$(STD) -c -O3 src/nasal_parse.cpp -fno-exceptions -fPIC -o build/nasal_parse.o -I .

build/optimizer.o: \
	src/nasal.h\
	src/nasal_err.h\
	src/nasal_ast.h\
	src/ast_visitor.h\
	src/optimizer.h src/optimizer.cpp src/nasal_ast.h | build
	$(CXX) -std=$(STD) -c -O3 src/optimizer.cpp -fno-exceptions -fPIC -o build/optimizer.o -I .

build/symbol_finder.o: \
	src/nasal.h\
	src/nasal_err.h\
	src/nasal_ast.h\
	src/ast_visitor.h\
	src/symbol_finder.h src/symbol_finder.cpp src/nasal_ast.h | build
	$(CXX) -std=$(STD) -c -O3 src/symbol_finder.cpp -fno-exceptions -fPIC -o build/symbol_finder.o -I .

build/ast_visitor.o: \
	src/nasal.h\
	src/nasal_err.h\
	src/nasal_ast.h\
	src/ast_visitor.h src/ast_visitor.cpp | build
	$(CXX) -std=$(STD) -c -O3 src/ast_visitor.cpp -fno-exceptions -fPIC -o build/ast_visitor.o -I .

build/ast_dumper.o: \
	src/nasal.h\
	src/nasal_err.h\
	src/nasal_ast.h\
	src/ast_visitor.h\
	src/ast_dumper.h src/ast_dumper.cpp | build
	$(CXX) -std=$(STD) -c -O3 src/ast_dumper.cpp -fno-exceptions -fPIC -o build/ast_dumper.o -I .

build/nasal_vm.o: $(NASAL_HEADER) src/nasal_vm.h src/nasal_vm.cpp | build
	$(CXX) -std=$(STD) -c -O3 src/nasal_vm.cpp -fno-exceptions -fPIC -o build/nasal_vm.o -I .

build/nasal_dbg.o: $(NASAL_HEADER) src/nasal_dbg.h src/nasal_dbg.cpp | build
	$(CXX) -std=$(STD) -c -O3 src/nasal_dbg.cpp -fno-exceptions -fPIC -o build/nasal_dbg.o -I .

.PHONY: clean
clean:
	@ echo "[clean] nasal" && if [ -e nasal ]; then rm nasal; fi
	@ echo "[clean] nasal.exe" && if [ -e nasal.exe ]; then rm nasal.exe; fi
	@ rm $(NASAL_OBJECT)

.PHONY: test
test:nasal
	@ ./nasal -e test/ascii-art.nas
	@ ./nasal -t -d test/bfs.nas
	@ ./nasal -t test/bigloop.nas
	@ ./nasal -t test/bp.nas
	@ ./nasal -d test/calc.nas
	@ ./nasal -e test/choice.nas
	@ ./nasal -e test/class.nas
	@ ./nasal -t -d test/console3D.nas 20
	@ ./nasal -e test/coroutine.nas
	@ ./nasal -t -d test/datalog.nas
	@ ./nasal -e test/diff.nas
	@ ./nasal -e test/donuts.nas 15
	-@ ./nasal -d test/exception.nas
	@ ./nasal -t -d test/fib.nas
	@ ./nasal -e test/filesystem.nas
	@ ./nasal -t -d test/globals_test.nas
	@ ./nasal -d test/hexdump.nas
	@ ./nasal -e test/json.nas
	@ ./nasal -e test/leetcode1319.nas
	@ ./nasal -d test/lexer.nas
	@ ./nasal -d test/life.nas
	@ ./nasal -t test/loop.nas
	@ ./nasal -t test/mandelbrot.nas
	@ ./nasal -t test/md5.nas
	@ ./nasal -t -d test/md5compare.nas
	@ ./nasal -d test/module_test.nas
	@ ./nasal -e test/nasal_test.nas
	@ ./nasal -t -d test/occupation.nas 2
	@ ./nasal -t -d test/pi.nas
	@ ./nasal -t -d test/prime.nas
	@ ./nasal -e test/qrcode.nas
	@ ./nasal -t -d test/quick_sort.nas
	@ ./nasal -e test/scalar.nas hello world
	@ ./nasal -e test/trait.nas
	@ ./nasal -t -d test/turingmachine.nas
	@ ./nasal -d test/wavecollapse.nas
	@ ./nasal test/word_collector.nas test/md5compare.nas
	@ ./nasal -t -d test/ycombinator.nas