#ifndef __NASAL_TOKEN_H__
#define __NASAL_TOKEN_H__

#include "nasal_var.h"

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

class token_list
{
	private:
		token_unit *head;
		int list_range;
	public:
		token_list();
		~token_list();
		token_unit* get_head();
		int get_list_range();
		void print_line_token(const int);
		void delete_all();
		void append(const int,const int,std::string&);
		void print();
};

token_list nasal_lexer;
#endif
