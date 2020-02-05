#include "nasal.h"

resource_file resource;
nasal_lexer lexer;
nasal_parse parser;
abstract_syntax_tree libroot;
abstract_syntax_tree root;

nasal_symbol_table symtable;

std::string command;

int main()
{
#ifdef _WIN32
			std::cout<<">> [system] Windows system."<<std::endl;
#endif
#ifdef _linux_
			std::cout<<">> [system] Linux system."<<std::endl;
#endif
#ifdef TARGET_OS_MAC
			std::cout<<">> [system] MacOS system."<<std::endl;
#endif
	std::cout<<">> Nasal interpreter ver 2.0 ."<<std::endl;
	std::cout<<">> Code: https://github.com/ValKmjolnir/Nasal-Interpreter"<<std::endl;
	std::cout<<">> More info: http://wiki.flightgear.org/Nasal_scripting_language"<<std::endl;
	std::cout<<">> Input \"help\" to get help ."<<std::endl;
	while(1)
	{
		std::cout<<">> ";
		std::cin>>command;
		if(command=="help")
		{
			std::cout<<">> [\'file\'] input a file."<<std::endl;
			std::cout<<">> [cls   ] clear the screen."<<std::endl;
			std::cout<<">> [del   ] clear the resource code."<<std::endl;
			std::cout<<">> [lib   ] add lib file."<<std::endl;
			std::cout<<">> [rs    ] print resource code."<<std::endl;
			std::cout<<">> [lex   ] turn code into tokens."<<std::endl;
			std::cout<<">> [par   ] turn tokens into abstract syntax tree."<<std::endl;
			std::cout<<">> [ast   ] check the abstract syntax tree."<<std::endl;
			std::cout<<">> [sym   ] generate symbol table and print the information of it."<<std::endl;
			std::cout<<">> [run   ] run code."<<std::endl;
			std::cout<<">> [info  ] print lexer,parser and ast on screen."<<std::endl;
			std::cout<<">> [exit  ] quit nasal interpreter."<<std::endl;
		}
		else if(command=="cls")
		{
#ifdef _WIN32
			system("cls");
#endif
#ifdef _linux_
			system("clear");
#endif
#ifdef TARGET_OS_MAC
			system("clear");
#endif
		}
		else if(command=="del")
		{
			resource.delete_all_source();
			lexer.delete_all_tokens();
			parser.delete_all_elements();
			symtable.set_scope_clear();
			root.set_clear();
			std::cout<<">> [Delete] complete."<<std::endl;
		}
		else if(command=="lib")
		{
			libroot.set_clear();
			resource.load_lib_file();
			lexer.scanner(resource.get_source());
			lexer.generate_detail_token();
			if(!lexer.get_error())
			{
				parser.get_token_list(lexer.get_detail_token_list());
				parser.main_generate();
				if(!parser.get_error())
				{
					libroot=parser.get_root();
					std::cout<<">> [Lib] loaded."<<std::endl;
				}
				else
					std::cout<<">> [Lib-error] lib files have parse error(s),stop."<<std::endl;
			}
			else
				std::cout<<">> [Lib-error] lib files have lexer error(s),stop."<<std::endl;
			resource.delete_all_source();
			lexer.delete_all_tokens();
			parser.delete_all_elements();
			symtable.set_scope_clear();
		}
		else if(command=="rs")
		{
			resource.print_resource();
		}
		else if(command=="lex")
		{
			lexer.scanner(resource.get_source());
			lexer.generate_detail_token();
			if(!lexer.get_error())
				lexer.print_token_list();
			else
				std::cout<<">> [Lexer] error occurred,stop."<<std::endl;
		}
		else if(command=="par")
		{
			lexer.scanner(resource.get_source());
			lexer.generate_detail_token();
			if(!lexer.get_error())
			{
				parser.get_token_list(lexer.get_detail_token_list());
				parser.print_detail_token();
				parser.main_generate();
			}
			else
				std::cout<<">> [Lexer] error occurred,stop."<<std::endl;
		}
		else if(command=="ast")
		{
			lexer.scanner(resource.get_source());
			lexer.generate_detail_token();
			if(!lexer.get_error())
			{
				parser.get_token_list(lexer.get_detail_token_list());
				parser.main_generate();
				if(!parser.get_error())
				{
					symtable.set_scope_clear();
					symtable.symbol_table_main_generate(parser.get_root());
					parser.get_root().print_tree();
				}
				else
					std::cout<<">> [Parse] error occurred,stop."<<std::endl;
			}
			else
				std::cout<<">> [Lexer] error occurred,stop."<<std::endl;
		}
		else if(command=="sym")
		{
			lexer.scanner(resource.get_source());
			lexer.generate_detail_token();
			if(!lexer.get_error())
			{
				parser.get_token_list(lexer.get_detail_token_list());
				parser.main_generate();
				if(!parser.get_error())
				{
					symtable.set_scope_clear();
					symtable.symbol_table_main_generate(parser.get_root());
					symtable.print_symbol_table();
					if(symtable.get_error())
						std::cout<<">> [Symbol] error occurred,stop."<<std::endl;
				}
				else
					std::cout<<">> [Parse] error occurred,stop."<<std::endl;
			}
			else
				std::cout<<">> [Lexer] error occurred,stop."<<std::endl;
		}
		else if(command=="run")
		{
			lexer.scanner(resource.get_source());
			lexer.generate_detail_token();
			if(!lexer.get_error())
			{
				parser.get_token_list(lexer.get_detail_token_list());
				parser.main_generate();
				if(!parser.get_error())
				{
					symtable.set_scope_clear();
					symtable.symbol_table_main_generate(parser.get_root());
					if(!symtable.get_error())
						root=parser.get_root();// run code
					else
						std::cout<<">> [Symbol] error occurred,stop."<<std::endl;
				}
				else
					std::cout<<">> [Parse] error occurred,stop."<<std::endl;
			}
			else
				std::cout<<">> [Lexer] error occurred,stop."<<std::endl;
		}
		else if(command=="info")
		{
			lexer.scanner(resource.get_source());
			lexer.print_token_list();
			lexer.generate_detail_token();
			parser.get_token_list(lexer.get_detail_token_list());
			parser.print_detail_token();
			parser.main_generate();
			parser.get_root().print_tree();
		}
		else if(command=="exit")
			break;
		else
			resource.input_file(command);
	}
	return 0;
}
