#ifndef __NASAL_RUNTIME_H__
#define __NASAL_RUNTIME_H__

enum runtime_returned_state
{
    rt_return=1,
    rt_break,
    rt_continue,
    rt_error,
    rt_exit_without_error
};

class nasal_runtime
{
private:
    // global_scope_address and function_return_address are addresses in garbage_collector_memory
    int function_returned_address;
    int global_scope_address;
    nasal_ast root;
    // process functions are private

    // generate number and return gc place of this number
    int number_generation(nasal_ast&);
    // generate string and return gc place of this string
    int string_generation(nasal_ast&);
    // generate vector and return gc place of this vector
    int vector_generation(nasal_ast&);
    // generate hash and return gc place of this hash
    int hash_generation(nasal_ast&);
    // generate function and return gc place of this function
    int function_generation(nasal_ast&);

    // main expression block running process
    int main_progress();
    // function/loop/conditional expression block running process
    int block_progress();
    // run loop
    int loop_progress();
    // run conditional
    int conditional_progress();
    // run function
    int function_progress();
    // get scalars in complex data structure like vector/hash/function/closure(scope)
    int call_nasal_scalar(nasal_ast&);
    // get scalars' memory place in complex data structure like vector/hash/function/closure(scope)
    int call_scalar_mem(nasal_ast&);
    // calculate scalars
    int calculation(nasal_ast&);
public:
    nasal_runtime();
    ~nasal_runtime();
    void set_root(nasal_ast&);
    void run();
};

nasal_runtime::nasal_runtime()
{
    this->root.clear();
    this->global_scope_address=-1;
    return;
}
nasal_runtime::~nasal_runtime()
{
    this->root.clear();
    this->global_scope_address=-1;
    return;
}
void nasal_runtime::set_root(nasal_ast& parse_result)
{
    this->root=parse_result;
    return;
}
void nasal_runtime::run()
{
    this->global_scope_address=nasal_vm.gc_alloc();
    nasal_vm.gc_get(global_scope_address).set_type(vm_closure);
    nasal_vm.gc_get(global_scope_address).get_closure().add_scope();

    nasal_vm.gc_get(global_scope_address).get_closure().del_scope();
    nasal_vm.del_reference(global_scope_address);
    return;
}

// private functions
int nasal_runtime::number_generation(nasal_ast& node)
{
    int new_addr=nasal_vm.gc_alloc();
    nasal_vm.gc_get(new_addr).set_type(vm_number);
    nasal_vm.gc_get(new_addr).set_number(trans_string_to_number(node.get_str()));
    return new_addr;
}
int nasal_runtime::string_generation(nasal_ast& node)
{
    int new_addr=nasal_vm.gc_alloc();
    nasal_vm.gc_get(new_addr).set_type(vm_string);
    nasal_vm.gc_get(new_addr).set_string(node.get_str());
    return new_addr;
}
int nasal_runtime::vector_generation(nasal_ast& node)
{
    int new_addr=nasal_vm.gc_alloc();
    nasal_vm.gc_get(new_addr).set_type(vm_vector);
    nasal_vector& ref_of_this_vector=nasal_vm.gc_get(new_addr).get_vector();

    int node_children_size=node.get_children().size();
    for(int i=0;i<node_children_size;++i)
    {
        int new_memory_space=nasal_vm.mem_alloc();
        ref_of_this_vector.add_elem(new_memory_space);
        switch(node.get_children()[i].get_type())
        {
            case ast_number:   nasal_vm.mem_init(new_memory_space,number_generation(node.get_children()[i]));break;
            case ast_string:   nasal_vm.mem_init(new_memory_space,string_generation(node.get_children()[i]));break;
            case ast_vector:   nasal_vm.mem_init(new_memory_space,vector_generation(node.get_children()[i]));break;
            case ast_hash:     nasal_vm.mem_init(new_memory_space,hash_generation(node.get_children()[i]));break;
            case ast_function: nasal_vm.mem_init(new_memory_space,function_generation(node.get_children()[i]));break;
        }
    }
    return new_addr;
}
int nasal_runtime::hash_generation(nasal_ast& node)
{
    int new_addr=nasal_vm.gc_alloc();
    nasal_vm.gc_get(new_addr).set_type(vm_hash);
    nasal_hash& ref_of_this_hash=nasal_vm.gc_get(new_addr).get_hash();

    int node_children_size=node.get_children().size();
    for(int i=0;i<node_children_size;++i)
    {
        int new_memory_space=nasal_vm.mem_alloc();
        std::string hash_member_name=node.get_children()[i].get_children()[0].get_str();
        ref_of_this_hash.add_elem(hash_member_name,new_memory_space);
        nasal_ast& ref_of_hash_member_value=node.get_children()[i].get_children()[1];
        switch(ref_of_hash_member_value.get_type())
        {
            case ast_number:   nasal_vm.mem_init(new_memory_space,number_generation(ref_of_hash_member_value));break;
            case ast_string:   nasal_vm.mem_init(new_memory_space,string_generation(ref_of_hash_member_value));break;
            case ast_vector:   nasal_vm.mem_init(new_memory_space,vector_generation(ref_of_hash_member_value));break;
            case ast_hash:     nasal_vm.mem_init(new_memory_space,hash_generation(ref_of_hash_member_value));break;
            case ast_function: nasal_vm.mem_init(new_memory_space,function_generation(ref_of_hash_member_value));break;
        }
    }
    return new_addr;
}
int nasal_runtime::function_generation(nasal_ast& node)
{
    int new_addr=nasal_vm.gc_alloc();
    nasal_vm.gc_get(new_addr).set_type(vm_function);
    nasal_function& ref_of_this_function=nasal_vm.gc_get(new_addr).get_func();

    ref_of_this_function.set_arguments(node.get_children()[0]);
    ref_of_this_function.set_run_block(node.get_children()[1]);
    return new_addr;
}
int nasal_runtime::main_progress()
{
    int ret_state=rt_exit_without_error;

    return ret_state;
}
int nasal_runtime::block_progress()
{
    int ret_state=rt_exit_without_error;
    return ret_state;
}
int nasal_runtime::loop_progress()
{
    int ret_state=rt_exit_without_error;
    return ret_state;
}
int nasal_runtime::conditional_progress()
{
    int ret_state=rt_exit_without_error;
    return ret_state;
}
int nasal_runtime::function_progress()
{
    int ret_state=rt_exit_without_error;
    return ret_state;
}
int nasal_runtime::call_nasal_scalar(nasal_ast& node)
{
    // unfinished
    return -1;
}
int nasal_runtime::call_scalar_mem(nasal_ast& node)
{
    // unfinished
    return -1;
}
int nasal_runtime::calculation(nasal_ast& node)
{
    int ret_address=-1;
    int calculation_type=node.get_type();
    if(calculation_type==ast_number)
        ret_address=number_generation(node);
    else if(calculation_type==ast_string)
        ret_address=string_generation(node);
    else if(calculation_type==ast_vector)
        ret_address=vector_generation(node);
    else if(calculation_type==ast_hash)
        ret_address=hash_generation(node);
    else if(calculation_type==ast_function)
        ret_address=function_generation(node);
    else if(calculation_type==ast_call)
        ret_address=call_nasal_scalar(node);
    else if(calculation_type==ast_add)
    {
        int left_gc_addr=calculation(node.get_children()[0]);
        int right_gc_addr=calculation(node.get_children()[1]);
        ret_address=nasal_scalar_calculator.nasal_scalar_add(left_gc_addr,right_gc_addr);
    }
    else if(calculation_type==ast_sub)
    {
        int left_gc_addr=calculation(node.get_children()[0]);
        int right_gc_addr=calculation(node.get_children()[1]);
        ret_address=nasal_scalar_calculator.nasal_scalar_sub(left_gc_addr,right_gc_addr);
    }
    else if(calculation_type==ast_mult)
    {
        int left_gc_addr=calculation(node.get_children()[0]);
        int right_gc_addr=calculation(node.get_children()[1]);
        ret_address=nasal_scalar_calculator.nasal_scalar_mult(left_gc_addr,right_gc_addr);
    }
    else if(calculation_type==ast_div)
    {
        int left_gc_addr=calculation(node.get_children()[0]);
        int right_gc_addr=calculation(node.get_children()[1]);
        ret_address=nasal_scalar_calculator.nasal_scalar_div(left_gc_addr,right_gc_addr);
    }
    else if(calculation_type==ast_link)
    {
        int left_gc_addr=calculation(node.get_children()[0]);
        int right_gc_addr=calculation(node.get_children()[1]);
        ret_address=nasal_scalar_calculator.nasal_scalar_link(left_gc_addr,right_gc_addr);
    }
    else if(calculation_type==ast_cmp_equal)
    {
        int left_gc_addr=calculation(node.get_children()[0]);
        int right_gc_addr=calculation(node.get_children()[1]);
        ret_address=nasal_scalar_calculator.nasal_scalar_cmp_equal(left_gc_addr,right_gc_addr);
    }
    else if(calculation_type==ast_cmp_not_equal)
    {
        int left_gc_addr=calculation(node.get_children()[0]);
        int right_gc_addr=calculation(node.get_children()[1]);
        ret_address=nasal_scalar_calculator.nasal_scalar_cmp_not_equal(left_gc_addr,right_gc_addr);
    }
    else if(calculation_type==ast_less_than)
    {
        int left_gc_addr=calculation(node.get_children()[0]);
        int right_gc_addr=calculation(node.get_children()[1]);
        ret_address=nasal_scalar_calculator.nasal_scalar_cmp_less(left_gc_addr,right_gc_addr);
    }
    else if(calculation_type==ast_less_equal)
    {
        int left_gc_addr=calculation(node.get_children()[0]);
        int right_gc_addr=calculation(node.get_children()[1]);
        ret_address=nasal_scalar_calculator.nasal_scalar_cmp_less_or_equal(left_gc_addr,right_gc_addr);
    }
    else if(calculation_type==ast_greater_than)
    {
        int left_gc_addr=calculation(node.get_children()[0]);
        int right_gc_addr=calculation(node.get_children()[1]);
        ret_address=nasal_scalar_calculator.nasal_scalar_cmp_greater(left_gc_addr,right_gc_addr);
    }
    else if(calculation_type==ast_greater_equal)
    {
        int left_gc_addr=calculation(node.get_children()[0]);
        int right_gc_addr=calculation(node.get_children()[1]);
        ret_address=nasal_scalar_calculator.nasal_scalar_cmp_greater_or_equal(left_gc_addr,right_gc_addr);
    }
    else if(calculation_type==ast_unary_not)
    {
        int addr=calculation(node.get_children()[0]);
        ret_address=nasal_scalar_calculator.nasal_scalar_unary_not(addr);
    }
    else if(calculation_type==ast_unary_sub)
    {
        int addr=calculation(node.get_children()[0]);
        ret_address=nasal_scalar_calculator.nasal_scalar_unary_sub(addr);
    }
    else if(calculation_type==ast_trinocular)
    {
        int condition_addr=calculation(node.get_children()[0]);
        int ret_1_addr=calculation(node.get_children()[1]);
        int ret_2_addr=calculation(node.get_children()[2]);
        int check_null=nasal_scalar_calculator.nasal_scalar_unary_not(condition_addr);
        if(nasal_vm.gc_get(check_null).get_number()==0)
        {
            ret_address=ret_1_addr;
            nasal_vm.del_reference(ret_2_addr);
        }
        else
        {
            ret_address=ret_2_addr;
            nasal_vm.del_reference(ret_1_addr);
        }
        nasal_vm.del_reference(condition_addr);
        nasal_vm.del_reference(check_null);
    }
    // unfinished
    else if(calculation_type==ast_equal)
    {
        int scalar_mem_space=call_scalar_mem(node.get_children()[0]);
        int new_scalar_gc_addr=calculation(node.get_children()[1]);
    }
    else if(calculation_type==ast_add_equal)
    {
        int scalar_mem_space=call_scalar_mem(node.get_children()[0]);
        int new_scalar_gc_addr=calculation(node.get_children()[1]);
    }
    else if(calculation_type==ast_sub_equal)
    {
        int scalar_mem_space=call_scalar_mem(node.get_children()[0]);
        int new_scalar_gc_addr=calculation(node.get_children()[1]);
    }
    else if(calculation_type==ast_div_equal)
    {
        int scalar_mem_space=call_scalar_mem(node.get_children()[0]);
        int new_scalar_gc_addr=calculation(node.get_children()[1]);
    }
    else if(calculation_type==ast_mult_equal)
    {
        int scalar_mem_space=call_scalar_mem(node.get_children()[0]);
        int new_scalar_gc_addr=calculation(node.get_children()[1]);
    }
    else if(calculation_type==ast_link_equal)
    {
        int scalar_mem_space=call_scalar_mem(node.get_children()[0]);
        int new_scalar_gc_addr=calculation(node.get_children()[1]);
    }
    return ret_address;
}

#endif