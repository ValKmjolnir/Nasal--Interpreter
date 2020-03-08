#ifndef __NASAL_RUNTIME_H__
#define __NASAL_RUNTIME_H__

class nasal_runtime
{
    private:
        // global scope is a hash_map
        // main_local_scope is used in main block 
        // when calling a loop or conditional expression
        // when adding a new identifier into scope,you need to check if this identifier is already in the scope.
        std::map<std::string,int> global_scope;
        std::list<std::map<std::string,int> > main_local_scope;
        
        // see detail of each enum type in function error_interrupt(const int)
        enum runtime_error_type
        {
            __incorrect_head_of_tree,
            __incorrect_head_of_func,
            __stack_overflow,
        };
        int  runtime_error_exit_mark;
        void error_interrupt    (const int);
        int  number_generation  (abstract_syntax_tree&);
        int  string_generation  (abstract_syntax_tree&);
        int  vector_generation  (std::list<std::map<std::string,int> >&,abstract_syntax_tree&);
        int  hash_generation    (std::list<std::map<std::string,int> >&,abstract_syntax_tree&);
        int  function_generation(std::list<std::map<std::string,int> >&,abstract_syntax_tree&);
        int  calculation        (std::list<std::map<std::string,int> >&,abstract_syntax_tree&);
        int  call_identifier    (std::list<std::map<std::string,int> >&,abstract_syntax_tree&);
        void assignment         (std::list<std::map<std::string,int> >&,abstract_syntax_tree&);
        void definition         (std::list<std::map<std::string,int> >&,abstract_syntax_tree&);
        void loop_expr          (std::list<std::map<std::string,int> >&,abstract_syntax_tree&);
        void conditional        (std::list<std::map<std::string,int> >&,abstract_syntax_tree&);
        void func_proc          (std::list<std::map<std::string,int> >&,abstract_syntax_tree&);
    public:
        nasal_runtime()
        {
            global_scope.clear();
            nasal_gc.gc_init();
            return;
        }
        ~nasal_runtime()
        {
            global_scope.clear();
            nasal_gc.gc_init();
            return;
        }
        void main_proc(abstract_syntax_tree&);
};

void nasal_runtime::error_interrupt(const int type)
{
    std::string error_head=">> [Runtime] fatal error: ";
    switch (type)
    {
        case __incorrect_head_of_tree:
            std::cout<<error_head<<"the abstract syntax tree's root type is not \'__root\'.please make sure that lib is loaded."<<std::endl;break;
        case __incorrect_head_of_func:
            std::cout<<error_head<<"called identifier is not a function."<<std::endl;break;
        case __stack_overflow:
            std::cout<<error_head<<"stack overflow."<<std::endl;break;
        default:
            std::cout<<error_head<<"unknown error."<<std::endl;break;
    }
    runtime_error_exit_mark=type;
    return;
}

int nasal_runtime::number_generation(abstract_syntax_tree& node)
{
    int addr=nasal_gc.gc_alloc();
    nasal_gc.get_scalar(addr).set_type(scalar_number);
    nasal_gc.get_scalar(addr).get_number().set_number(node.get_var_number());
    return addr;
}
int nasal_runtime::string_generation(abstract_syntax_tree& node)
{
    int addr=nasal_gc.gc_alloc();
    nasal_gc.get_scalar(addr).set_type(scalar_string);
    nasal_gc.get_scalar(addr).get_string().set_string(node.get_var_string());
    return addr;
}
int nasal_runtime::vector_generation(std::list<std::map<std::string,int> >& local_scope,abstract_syntax_tree& node)
{
    int addr=nasal_gc.gc_alloc();
    nasal_gc.get_scalar(addr).set_type(scalar_vector);
    for(std::list<abstract_syntax_tree>::iterator i=node.get_children().begin();i!=node.get_children().end();++i)
    {
        int var_type=i->get_node_type();
        if(var_type==__number)
            nasal_gc.get_scalar(addr).get_vector().vec_push(number_generation(*i));
        else if(var_type==__string)
            nasal_gc.get_scalar(addr).get_vector().vec_push(string_generation(*i));
        else if(var_type==__vector)
            nasal_gc.get_scalar(addr).get_vector().vec_push(vector_generation(local_scope,*i));
        else if(var_type==__hash)
            nasal_gc.get_scalar(addr).get_vector().vec_push(hash_generation(local_scope,*i));
        else if(var_type==__function)
            nasal_gc.get_scalar(addr).get_vector().vec_push(function_generation(local_scope,*i));
        else if(var_type==__id)
            nasal_gc.get_scalar(addr).get_vector().vec_push(call_identifier(local_scope,*i));
        else
        {
            ;
        }
    }
    return addr;
}
int nasal_runtime::hash_generation(std::list<std::map<std::string,int> >& local_scope,abstract_syntax_tree& node)
{
    int addr=nasal_gc.gc_alloc();
    nasal_gc.get_scalar(addr).set_type(scalar_hash);
    nasal_gc.get_scalar(addr).get_hash().set_self_addr(addr);
    for(std::list<abstract_syntax_tree>::iterator i=node.get_children().begin();i!=node.get_children().end();++i)
    {
        if(i->get_node_type()!=__hash_member)
            break;
        else
        {
            std::string member_name=i->get_children().front().get_var_string();
            int var_type=i->get_children().back().get_node_type();
            if(var_type==__number)
                ;
            else if(var_type==__string)
                ;
            else if(var_type==__vector)
                ;
            else if(var_type==__hash)
                ;
            else if(var_type==__function)
            {
                // hash's function must get a parent_hash_addr 
                // this address will be given to identifier 'me'
                nasal_gc.get_scalar(addr).get_hash().hash_push(member_name,function_generation(local_scope,i->get_children().back()));
                nasal_gc.get_scalar(nasal_gc.get_scalar(addr).get_hash().get_hash_member(member_name)).get_function().set_parent_hash_addr(addr);
            }
            else
            {
                ;
            }
        }
    }
    return addr;
}
int nasal_runtime::function_generation(std::list<std::map<std::string,int> >& local_scope,abstract_syntax_tree& node)
{
    int addr=nasal_gc.gc_alloc();
    nasal_gc.get_scalar(addr).set_type(scalar_function);
    nasal_gc.get_scalar(addr).get_function().set_local_scope(local_scope);
    nasal_gc.get_scalar(addr).get_function().set_statement_block(node);
    return addr;
}
int nasal_runtime::calculation(std::list<std::map<std::string,int> >& local_scope,abstract_syntax_tree& node)
{
    int operator_type=node.get_node_type();
    if(operator_type==__add_operator)
    {
        node.get_children().front();
        node.get_children().back();
    }
    return -1;
}
int nasal_runtime::call_identifier(std::list<std::map<std::string,int> >& local_scope,abstract_syntax_tree& node)
{
    if(local_scope.empty())
        ;
    return -1;
}
void nasal_runtime::assignment(std::list<std::map<std::string,int> >& local_scope,abstract_syntax_tree& node)
{
    if(local_scope.empty())
        ;
    return;
}
void nasal_runtime::definition(std::list<std::map<std::string,int> >& local_scope,abstract_syntax_tree& node)
{
    if(local_scope.empty())
        ;
    return;
}
void nasal_runtime::loop_expr(std::list<std::map<std::string,int> >& local_scope,abstract_syntax_tree& node)
{
    std::map<std::string,int> new_scope;
    local_scope.push_back(new_scope);

    return;
}
void nasal_runtime::conditional(std::list<std::map<std::string,int> >& local_scope,abstract_syntax_tree& node)
{
    std::map<std::string,int> new_scope;
    local_scope.push_back(new_scope);
    
    return;
}

void nasal_runtime::func_proc(std::list<std::map<std::string,int> >& local_scope,abstract_syntax_tree& func_root)
{
    if(func_root.get_node_type()!=__function)
    {
        error_interrupt(__incorrect_head_of_func);
        return;
    }
    std::map<std::string,int> new_scope;
    local_scope.push_back(new_scope);
    for(std::list<abstract_syntax_tree>::iterator iter=func_root.get_children().front().get_children().begin();iter!=func_root.get_children().front().get_children().end();++iter)
    {

    }
    for(std::list<abstract_syntax_tree>::iterator iter=func_root.get_children().back().get_children().begin();iter!=func_root.get_children().back().get_children().end();++iter)
    {
        // use local value node_type to avoid calling function too many times.
        int node_type=iter->get_node_type();
        if(node_type==__number || node_type==__string)
            ;
        // only number or string
        else if(node_type==__id)
            this->call_identifier(local_scope,*iter);
        else if(node_type==__vector)
            this->vector_generation(local_scope,*iter);
        else if(node_type==__hash)
            this->hash_generation(local_scope,*iter);
        else if(node_type==__function)
            this->function_generation(local_scope,*iter);
        else if(node_type==__add_operator  || node_type==__sub_operator || node_type==__mul_operator  || node_type==__div_operator || node_type==__link_operator ||
                node_type==__cmp_equal || node_type==__cmp_less || node_type==__cmp_more || node_type==__cmp_not_equal || node_type==__cmp_less_or_equal || node_type==__cmp_more_or_equal ||
                node_type==__and_operator || node_type==__or_operator || node_type==__ques_mark ||
                node_type==__equal || node_type==__add_equal || node_type==__sub_equal || node_type==__div_equal || node_type==__mul_equal || node_type==__link_equal)
            this->calculation(local_scope,*iter);
        else if(node_type==__definition)
            this->definition(local_scope,*iter);
        else if(node_type==__conditional)
            this->conditional(local_scope,*iter);
        else if((node_type==__while) || (node_type==__for) || (node_type==__foreach) || (node_type==__forindex))
            this->loop_expr(local_scope,*iter);
        if(runtime_error_exit_mark>=0)
            break;
    }
    return;
}

void nasal_runtime::main_proc(abstract_syntax_tree& root)
{
    time_t begin_time,end_time;
    begin_time=std::time(NULL);

    // initializing global scope and nasal_gc
    // runtime_error_exit_mark is set to -1,if runtime_error_exit_mark >=0,this means an error occurred
    global_scope.clear();
    nasal_gc.gc_init();
    runtime_error_exit_mark=-1;

    if(root.get_node_type()!=__root)
    {
        error_interrupt(__incorrect_head_of_tree);
        return;
    }
    for(std::list<abstract_syntax_tree>::iterator iter=root.get_children().begin();iter!=root.get_children().end();++iter)
    {
        // use local value node_type to avoid calling function too many times.
        int node_type=iter->get_node_type();
        if(node_type==__number || node_type==__string)
            ;
        else if(node_type==__id)
            this->call_identifier(main_local_scope,*iter);
        else if(node_type==__vector)
            this->vector_generation(main_local_scope,*iter);
        else if(node_type==__hash)
            this->hash_generation(main_local_scope,*iter);
        else if(node_type==__function)
            this->function_generation(main_local_scope,*iter);
        else if(node_type==__add_operator  || node_type==__sub_operator || node_type==__mul_operator  || node_type==__div_operator || node_type==__link_operator ||
                node_type==__cmp_equal || node_type==__cmp_less || node_type==__cmp_more || node_type==__cmp_not_equal || node_type==__cmp_less_or_equal || node_type==__cmp_more_or_equal ||
                node_type==__and_operator || node_type==__or_operator || node_type==__ques_mark ||
                node_type==__equal || node_type==__add_equal || node_type==__sub_equal || node_type==__div_equal || node_type==__mul_equal || node_type==__link_equal)
            this->calculation(main_local_scope,*iter);
        else if(node_type==__definition)
            this->definition(main_local_scope,*iter);
        else if(node_type==__conditional)
            this->conditional(main_local_scope,*iter);
        else if((node_type==__while) || (node_type==__for) || (node_type==__foreach) || (node_type==__forindex))
            this->loop_expr(main_local_scope,*iter);
        if(runtime_error_exit_mark>=0)
            break;
    }

    end_time=std::time(NULL);
    std::cout<<">> [Runtime] process exited after "<<end_time-begin_time<<" s ."<<std::endl;
    return;
}

#endif