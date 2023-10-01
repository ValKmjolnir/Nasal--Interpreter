#include "nasal.h"
#include "nasal_err.h"
#include "nasal_lexer.h"
#include "nasal_ast.h"
#include "nasal_parse.h"
#include "nasal_import.h"
#include "ast_visitor.h"
#include "ast_dumper.h"
#include "symbol_finder.h"
#include "optimizer.h"
#include "nasal_codegen.h"
#include "nasal_vm.h"
#include "nasal_dbg.h"
#include "repl.h"

#include <unordered_map>
#include <thread>
#include <cstdlib>

const u32 VM_RAW_AST   = 1;
const u32 VM_AST       = 1<<1;
const u32 VM_CODE      = 1<<2;
const u32 VM_TIME      = 1<<3;
const u32 VM_EXEC      = 1<<4;
const u32 VM_DETAIL    = 1<<5;
const u32 VM_DEBUG     = 1<<6;
const u32 VM_SYMINFO   = 1<<7;
const u32 VM_PROFILE   = 1<<8;
const u32 VM_PROF_ALL  = 1<<9;

std::ostream& help(std::ostream& out) {
    out
    << "\n"
    << "     ,--#-,\n"
    << "<3  / \\____\\  <3\n"
    << "    |_|__A_|\n"
#ifdef _WIN32
    << "use command <chcp 65001> to use unicode.\n"
#endif
    << "\nnasal <option>\n"
    << "option:\n"
    << "   -h,   --help     | get help.\n"
    << "   -v,   --version  | get version.\n"
    << "   -r,   --repl     | use repl interpreter(experimental).\n"
    << "\nnasal [option] <file> [argv]\n"
    << "option:\n"
    << "   -a,   --ast      | view ast after link/optimize process.\n"
    << "         --raw-ast  | view ast without after-processing.\n"
    << "   -c,   --code     | view generated bytecode.\n"
    << "   -s,   --symbol   | show analysed symbol info.\n"
    << "   -e,   --exec     | execute directly.\n"
    << "   -t,   --time     | show execute time.\n"
    << "   -d,   --detail   | get detail info.\n"
    << "   -dbg, --debug    | debug mode.\n"
    << "         --prof     | show profiling result, available in debug mode.\n"
    << "         --prof-all | show profiling result of all files,"
    << "available under debug mode.\n"
    << "file:\n"
    << "   <filename>       | execute file.\n"
    << "argv:\n"
    << "   <args>           | cmd arguments used in program.\n"
    << "\n";
    return out;
}

std::ostream& logo(std::ostream& out) {
    out
    << "\n"
    << "       __                _\n"
    << "    /\\ \\ \\__ _ ___  __ _| |\n"
    << "   /  \\/ / _` / __|/ _` | |\n"
    << "  / /\\  / (_| \\__ \\ (_| | |\n"
    << "  \\_\\ \\/ \\__,_|___/\\__,_|_|\n"
    << "ver  : " << __nasver << " (" << __DATE__ << " " << __TIME__ << ")\n"
    << "std  : c++ " << __cplusplus << "\n"
    << "core : " << std::thread::hardware_concurrency() << " core(s)\n"
    << "repo : https://github.com/ValKmjolnir/Nasal-Interpreter\n"
    << "repo : https://gitee.com/valkmjolnir/Nasal-Interpreter\n"
    << "wiki : https://wiki.flightgear.org/Nasal_scripting_language\n"
    << "\n"
    << "input <nasal -h> to get help .\n\n";
    return out;
}

std::ostream& version(std::ostream& out) {
    std::srand(std::time(nullptr));
    f64 num = 0;
    for(u32 i = 0; i<5; ++i) {
        num = (num+rand())*(1.0/(RAND_MAX+1.0));
    }
    if (num<0.01) {
        nasal::parse::easter_egg();
    }
    out << "nasal interpreter version " << __nasver;
    out << " (" << __DATE__ << " " << __TIME__ << ")\n";
    return out;
}

[[noreturn]]
void err() {
    std::cerr
    << "invalid argument(s).\n"
    << "use <nasal -h> to get help.\n";
    std::exit(1);
}

void execute(
    const std::string& file,
    const std::vector<std::string>& argv,
    const u32 cmd) {

    using clk = std::chrono::high_resolution_clock;
    const auto den = clk::duration::period::den;

    nasal::lexer   lex;
    nasal::parse   parse;
    nasal::linker  ld;
    nasal::codegen gen;

    // lexer scans file to get tokens
    lex.scan(file).chkerr();

    // parser gets lexer's token list to compile
    parse.compile(lex).chkerr();
    if (cmd&VM_RAW_AST) {
        auto dumper = std::unique_ptr<nasal::ast_dumper>(new nasal::ast_dumper);
        dumper->dump(parse.tree());
    }

    // linker gets parser's ast and load import files to this ast
    ld.link(parse, file, cmd&VM_DETAIL).chkerr();
    
    // optimizer does simple optimization on ast
    auto opt = std::unique_ptr<nasal::optimizer>(new nasal::optimizer);
    opt->do_optimization(parse.tree());
    if (cmd&VM_AST) {
        auto dumper = std::unique_ptr<nasal::ast_dumper>(new nasal::ast_dumper);
        dumper->dump(parse.tree());
    }

    // code generator gets parser's ast and import file list to generate code
    gen.compile(parse, ld, false).chkerr();
    if (cmd&VM_CODE) {
        gen.print(std::cout);
    }
    if (cmd&VM_SYMINFO) {
        gen.symbol_dump(std::cout);
    }

    // run
    auto start = clk::now();
    if (cmd&VM_DEBUG) {
        auto debugger = std::unique_ptr<nasal::dbg>(new nasal::dbg);
        debugger->run(gen, ld, argv, cmd&VM_PROFILE, cmd&VM_PROF_ALL);
    } else if (cmd&VM_TIME || cmd&VM_EXEC) {
        auto runtime = std::unique_ptr<nasal::vm>(new nasal::vm);
        runtime->set_detail_report_info(cmd&VM_DETAIL);
        runtime->run(gen, ld, argv);
    }

    // get running time
    auto end = clk::now();
    if (cmd&VM_TIME) {
        std::clog << "process exited after ";
        std::clog << (end-start).count()*1.0/den << "s.\n\n";
    }
}

i32 main(i32 argc, const char* argv[]) {
    // output version info
    if (argc<=1) {
        std::clog << logo;
        return 0;
    }

    // run directly or show help
    if (argc==2) {
        std::string s(argv[1]);
        if (s=="-h" || s=="--help") {
            std::clog << help;
        } else if (s=="-v" || s=="--version") {
            std::clog << version;
        } else if (s=="-r" || s=="--repl") {
            auto repl = std::unique_ptr<nasal::repl::repl>(new nasal::repl::repl);
            repl->execute();
        } else if (s[0]!='-') {
            execute(s, {}, VM_EXEC);
        } else {
            err();
        }
        return 0;
    }

    // execute with arguments
    const std::unordered_map<std::string, u32> cmdlst = {
        {"--raw-ast", VM_RAW_AST},
        {"--ast", VM_AST},
        {"-a", VM_AST},
        {"--code", VM_CODE},
        {"-c", VM_CODE},
        {"--symbol", VM_SYMINFO},
        {"-s", VM_SYMINFO},
        {"--exec", VM_EXEC},
        {"-e", VM_EXEC},
        {"--time", VM_TIME|VM_EXEC},
        {"-t", VM_TIME|VM_EXEC},
        {"--detail", VM_DETAIL|VM_EXEC},
        {"-d", VM_DETAIL|VM_EXEC},
        {"--debug", VM_DEBUG},
        {"-dbg", VM_DEBUG},
        {"--prof", VM_PROFILE},
        {"--prof-all", VM_PROF_ALL}
    };
    u32 cmd = 0;
    std::string filename = "";
    std::vector<std::string> vm_argv;
    for(i32 i = 1; i<argc; ++i) {
        if (cmdlst.count(argv[i])) {
            cmd |= cmdlst.at(argv[i]);
        } else if (!filename.length()) {
            filename = argv[i];
        } else {
            vm_argv.push_back(argv[i]);
        }
    }
    if (!filename.length()) {
        err();
    }
    execute(filename, vm_argv, cmd? cmd:VM_EXEC);
    return 0;
}