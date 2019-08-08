#ifndef __NASAL_FUNCTIONAL_H__
#define __NASAL_FUNCTIONAL_H__

#include "nasal_var.h"

//for function
#define FUNC_BEGIN       0
#define FUNC_OPERATOR    1
#define FUNC_IDENTIFIER  2
#define FUNC_NUMBER      3
#define FUNC_RESERVEWORD 4
#define FUNC_STRING      5

//for token and lexer
#define OPERATOR    1 //��� or ����� 
#define IDENTIFIER  2 //�Զ����ʶ�� 
#define NUMBER      3 //���� 
#define RESERVEWORD 4 //�ؼ��� 
#define STRING      5 //�ַ������� 

struct token_unit
{
	int type;
	std::string content;
	int line;
	token_unit *next;
};

struct parameter
{
	var param_var;
	parameter *next;
};

class func
{
	private:
		token_unit *statement_head;
		parameter *parameter_head;
	public:
		func();
		func(const func&);
		~func();
		func& operator=(const func&);
		void append_var(var&);
		void append_token(const int,const int,std::string &);
};

class token_list
{
	private:
		token_unit *head;
	public:
		token_list();
		~token_list();
		void print_line_token(const int);
		void delete_all();
		void append(const int,const int,std::string&);
		void print();
};

#endif
