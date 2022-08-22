﻿#ifndef __NASAL_AST_H__
#define __NASAL_AST_H__

enum ast_node:u32
{
    ast_null=0,      // null node
    ast_root,        // mark the root node of ast
    ast_block,       // expression block 
    ast_file,        // used to store which file the sub-tree is on, only used in main block
    ast_nil,         // nil keyword
    ast_num,         // number, basic value type
    ast_str,         // string, basic value type
    ast_id,          // identifier
    ast_func,        // func keyword
    ast_hash,        // hash, basic value type
    ast_vec,         // vector, basic value type
    ast_pair,        // pair of key and value in hashmap
    ast_call,        // mark a sub-tree of calling an identifier
    ast_callh,       // id.name
    ast_callv,       // id[index]
    ast_callf,       // id()
    ast_subvec,      // id[index:index]
    ast_args,        // mark a sub-tree of function parameters
    ast_default,     // default parameter
    ast_dynamic,     // dynamic parameter
    ast_and,         // and keyword
    ast_or,          // or keyword
    ast_equal,       // =
    ast_addeq,       // +=
    ast_subeq,       // -=
    ast_multeq,      // *=
    ast_diveq,       // /=
    ast_lnkeq,       // ~=
    ast_cmpeq,       // ==
    ast_neq,         // !=
    ast_less,        // <
    ast_leq,         // <=
    ast_grt,         // >
    ast_geq,         // >=
    ast_add,         // +
    ast_sub,         // -
    ast_mult,        // *
    ast_div,         // /
    ast_link,        // ~
    ast_neg,         // -
    ast_not,         // ~
    ast_trino,       // ?:
    ast_for,         // for keyword
    ast_forindex,    // forindex keyword
    ast_foreach,     // foreach keyword
    ast_while,       // while
    ast_iter,        // iterator, used in forindex/foreach
    ast_conditional, // mark a sub-tree of conditional expression
    ast_if,          // if keyword
    ast_elsif,       // elsif keyword
    ast_else,        // else keyword
    ast_multi_id,    // multi identifiers sub-tree
    ast_multi_scalar,// multi value sub-tree
    ast_def,         // definition
    ast_multi_assign,// multi assignment sub-tree
    ast_continue,    // continue keyword, only used in loop
    ast_break,       // break keyword, only used in loop
    ast_ret          // return keyword, only used in function block
};

const char* ast_name[]=
{
    "null",
    "root",
    "block",
    "file",
    "nil",
    "num",
    "str",
    "id",
    "func",
    "hash",
    "vec",
    "pair",
    "call",
    "callh",
    "callv",
    "callf",
    "subvec",
    "args",
    "default",
    "dynamic",
    "and",
    "or",
    "=",
    "+=",
    "-=",
    "*=",
    "/=",
    "~=",
    "==",
    "!=",
    "<",
    "<=",
    ">",
    ">=",
    "+",
    "-",
    "*",
    "/",
    "~",
    "unary-",
    "unary!",
    "trino",
    "for",
    "forindex",
    "foreach",
    "while",
    "iter",
    "conditional",
    "if",
    "elsif",
    "else",
    "multi-id",
    "multi-scalar",
    "def",
    "multi-assign",
    "continue",
    "break",
    "return"
};

class nasal_ast
{
private:
    u32 _line;
    u32 _type;
    f64 _num;
    string _str;
    std::vector<nasal_ast> _child;
public:
    nasal_ast(const u32 l=0,const u32 t=ast_null):_line(l),_type(t),_num(0){}
    nasal_ast(const nasal_ast&);
    nasal_ast(nasal_ast&&);
    void tree();
    void print(u32,bool,std::vector<string>&);
    void clear();
    
    nasal_ast& operator=(const nasal_ast&);
    nasal_ast& operator=(nasal_ast&&);
    nasal_ast& operator[](usize n){return _child[n];}
    const nasal_ast& operator[](usize n) const {return _child[n];}
    usize size() const {return _child.size();}
    
    void add(nasal_ast&& ast){_child.push_back(std::move(ast));}
    void add(const nasal_ast& ast){_child.push_back(ast);}
    void set_line(const u32 l){_line=l;}
    void set_type(const u32 t){_type=t;}
    void set_str(const string& s){_str=s;}
    void set_num(const f64 n){_num=n;}

    inline u32 line() const {return _line;}
    inline u32 type() const {return _type;}
    inline f64 num()  const {return _num;}
    inline const string& str() const {return _str;}
    inline const std::vector<nasal_ast>& child() const {return _child;}
    inline std::vector<nasal_ast>& child(){return _child;}
};

nasal_ast::nasal_ast(const nasal_ast& tmp):
    _str(tmp._str),_child(tmp._child)
{
    _line=tmp._line;
    _type=tmp._type;
    _num =tmp._num;
}

nasal_ast::nasal_ast(nasal_ast&& tmp)
{
    _line=tmp._line;
    _type=tmp._type;
    _num =tmp._num;
    _str.swap(tmp._str);
    _child.swap(tmp._child);
}

nasal_ast& nasal_ast::operator=(const nasal_ast& tmp)
{
    _line=tmp._line;
    _type=tmp._type;
    _num=tmp._num;
    _str=tmp._str;
    _child=tmp._child;
    return *this;
}

nasal_ast& nasal_ast::operator=(nasal_ast&& tmp)
{
    _line=tmp._line;
    _type=tmp._type;
    _num=tmp._num;
    _str.swap(tmp._str);
    _child.swap(tmp._child);
    return *this;
}

void nasal_ast::clear()
{
    _line=0;
    _num=0;
    _str="";
    _type=ast_null;
    _child.clear();
}

void nasal_ast::tree()
{
    std::vector<string> tmp;
    print(0,false,tmp);
}

void nasal_ast::print(u32 depth,bool last,std::vector<string>& indent)
{
    for(auto& i:indent)
        std::cout<<i;
    std::cout<<ast_name[_type];
    if(_type==ast_str || _type==ast_id ||
       _type==ast_default || _type==ast_dynamic ||
       _type==ast_callh)
        std::cout<<":"<<rawstr(_str);
    else if(_type==ast_num || _type==ast_file)
        std::cout<<":"<<_num;
    std::cout<<'\n';
    if(last && depth)
        indent.back()="  ";
    else if(!last && depth)
#ifdef _WIN32
        indent.back()="| ";
#else
        indent.back()="│ ";
#endif
    for(u32 i=0;i<_child.size();++i)
    {
#ifdef _WIN32
        indent.push_back(i==_child.size()-1?"+-":"|-");
#else
        indent.push_back(i==_child.size()-1?"└─":"├─");
#endif
        _child[i].print(depth+1,i==_child.size()-1,indent);
        indent.pop_back();
    }
}

#endif