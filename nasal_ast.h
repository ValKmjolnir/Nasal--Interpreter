﻿#pragma once

#include <vector>
#include <cstring>

#include "nasal.h"

enum ast_node:u32 {
    ast_null=0,      // null node
    ast_root,        // mark the root node of ast
    ast_block,       // expression block 
    ast_file,        // used to store which file the sub-tree is on, only used in main block
    ast_nil,         // nil keyword
    ast_num,         // number, basic value type
    ast_str,         // string, basic value type
    ast_id,          // identifier
    ast_bool,        // bools
    ast_func,        // func keyword
    ast_hash,        // hash, basic value type
    ast_vec,         // vector, basic value type
    ast_pair,        // pair of key and value in hashmap
    ast_call,        // mark a sub-tree of calling an identifier
    ast_callh,       // id.name
    ast_callv,       // id[index]
    ast_callf,       // id()
    ast_subvec,      // id[index:index]
    ast_params,      // mark a sub-tree of function parameters
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
    ast_neg,         // unary -
    ast_lnot,        // unary !
    ast_bnot,        // unary ~ bitwise not
    ast_bitor,       // bitwise or
    ast_bitxor,      // bitwise xor
    ast_bitand,      // bitwise and
    ast_trino,       // ?:
    ast_for,         // for keyword
    ast_forindex,    // forindex keyword
    ast_foreach,     // foreach keyword
    ast_while,       // while
    ast_iter,        // iterator, used in forindex/foreach
    ast_cond,        // mark a sub-tree of conditional expression
    ast_if,          // if keyword
    ast_elsif,       // elsif keyword
    ast_else,        // else keyword
    ast_multi_id,    // multi identifiers sub-tree
    ast_tuple,       // tuple, only used in multiple assignment
    ast_def,         // definition
    ast_multi_assign,// multi assignment sub-tree
    ast_continue,    // continue keyword, only used in loop
    ast_break,       // break keyword, only used in loop
    ast_ret          // return keyword, only used in function block
};

const char* ast_name[]={
    "Null",
    "AbstractSyntaxTreeRoot",
    "CodeBlock",
    "FileIndex",
    "NilLiteral",
    "NumberLiteral",
    "StringLiteral",
    "Identifier",
    "BoolLiteral",
    "Function",
    "HashMap",
    "Vector",
    "HashMapPair",
    "IdentifierCallExpression",
    "HashMapCallExpression",
    "VectorCallExpression",
    "FunctionCallExpression",
    "SubVector",
    "ParameterList",
    "DefaultParameter",
    "DynamicParameter",
    "AndExpression",
    "OrExpression",
    "EqualExpression",
    "AddEqualExpression",
    "SubEqualExpression",
    "MultEqualExpression",
    "DivEqualExpression",
    "LinkEqualExpression",
    "CompareEqualExpression",
    "NotEqualExpression",
    "LessExpression",
    "LessOrEqualExpression",
    "GreatExpression",
    "GreatOrEqualExpression",
    "AddExpression",
    "SubExpression",
    "MultExpression",
    "DivExpression",     
    "LinkExpression",
    "NegativeExpression",
    "LogicalNotExpression",
    "BitwiseNotExpression",
    "BitwiseOrExpression",
    "BitwiseXorExpression",
    "BitwiseAndExpression",
    "TrinocularExpression",
    "ForLoop",
    "ForindexLoop",
    "ForeachLoop",
    "WhileLoop",
    "Iterator",
    "ConditionStatement",
    "IfStatement",
    "ElsifStatement",
    "ElseStatement",
    "LeftTuple",
    "Tuple",
    "Definition",
    "MultipleAssignment",
    "Continue",
    "Break",
    "ReturnStatement"
};

class ast {
private:
    u32 nd_line;
    u32 nd_col;
    u32 nd_type;
    f64 nd_num;
    string nd_file;
    string nd_str;
    std::vector<ast> nd_child;
public:
    ast(const u32 l,const u32 c,const u32 t,const std::string& f)
        : nd_line(l),nd_col(c),nd_type(t),nd_num(0),nd_file(f),nd_str("") {}
    ast(const ast&) = default;
    ast(ast&&) = default;
    ast& operator=(const ast&) = default;
    ast& operator=(ast&&) = default;

    void dump() const;
    void print(u32,bool,std::vector<string>&) const;
    void clear();

    ast& operator[](usize n) {return nd_child[n];}
    const ast& operator[](usize n) const {return nd_child[n];}
    usize size() const {return nd_child.size();}
    
    void add(ast&& node) {nd_child.push_back(std::move(node));}
    void add(const ast& node) {nd_child.push_back(node);}
    void set_line(const u32 l) {nd_line=l;}
    void set_type(const u32 t) {nd_type=t;}
    void set_str(const string& s) {nd_str=s;}
    void set_num(const f64 n) {nd_num=n;}

    u32 line() const {return nd_line;}
    u32 col()  const {return nd_col;}
    u32 type() const {return nd_type;}
    f64 num()  const {return nd_num;}
    const string& str() const {return nd_str;}
    const std::vector<ast>& child() const {return nd_child;}
    std::vector<ast>& child() {return nd_child;}
};

void ast::clear() {
    nd_line=nd_col=0;
    nd_num=0;
    nd_str.clear();
    nd_type=ast_null;
    nd_child.clear();
}

void ast::dump() const{
    std::vector<string> tmp;
    print(0,false,tmp);
}

void ast::print(u32 depth,bool last,std::vector<string>& indent) const{
    for(auto& i:indent) {
        std::cout<<i;
    }
    std::cout<<ast_name[nd_type];
    if (nd_type==ast_str ||
        nd_type==ast_id ||
        nd_type==ast_bool ||
        nd_type==ast_default ||
        nd_type==ast_dynamic ||
        nd_type==ast_callh) {
        std::cout<<":"<<rawstr(nd_str);
    } else if (nd_type==ast_num || nd_type==ast_file) {
        std::cout<<":"<<nd_num;
    }
    std::cout<<" --> "<<nd_file<<":"<<nd_line<<":"<<nd_col<<"\n";
    if (last && depth) {
        indent.back()="  ";
    } else if (!last && depth) {
        indent.back()=is_windows()? "| ":"│ ";
    }
    for(u32 i=0;i<nd_child.size();++i) {
        if (is_windows()) {
            indent.push_back(i==nd_child.size()-1?"+-":"|-");
        } else {
            indent.push_back(i==nd_child.size()-1?"└─":"├─");
        }
        nd_child[i].print(depth+1,i==nd_child.size()-1,indent);
        indent.pop_back();
    }
}
