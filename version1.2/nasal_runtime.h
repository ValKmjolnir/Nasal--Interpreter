#ifndef __NASAL_RUNTIME_H__
#define __NASAL_RUNTIME_H__

class var
{
	private:
		int type;
		std::string name;
		double number;
		std::string str;
		std::list<var> var_list;
		std::list<var> var_hash;
		abstract_syntax_tree function;
	public:
		var()
		{
			type=0;
			name="";
			number=0;
			str="";
			function.set_clear();
		}
		var(const var& p)
		{
			type=p.type;
			name=p.name;
			number=p.number;
			str=p.str;
			function=p.function;
		}
		var& operator=(const var& p)
		{
			type=p.type;
			name=p.name;
			number=p.number;
			str=p.str;
			function=p.function;
			return *this;
		}
		void print_information()
		{
			std::cout<<"[ type: ";print_token(type);std::cout<<" ]";
			std::cout<<"[ name: "<<name<<" ]";
			std::cout<<"[ number: "<<number<<" ]";
			std::cout<<"[ string: "<<str<<" ]";
			std::cout<<std::endl;
			return;
		}
		void set_type(const int var_type)
		{
			type=var_type;
			return;
		}
		void set_name(std::string var_name_str)
		{
			name=var_name_str;
			return;
		}
		void set_number(const double var_number)
		{
			number=var_number;
			return;
		}
		void set_string(std::string s)
		{
			str=s;
			return;
		}
		void set_function(abstract_syntax_tree func)
		{
			function=func;
			return;
		}
		void set_list(std::list<var> p)
		{
			var_list=p;
			return;
		}
		void append_list(var p)
		{
			var_list.push_back(p);
			return;
		}
		void set_hash(std::list<var> p)
		{
			var_hash=p;
			return;
		}
		void append_hash(var p)
		{
			var_hash.push_back(p);
			return;
		}
		std::string get_name()
		{
			return name;
		}
		double get_number()
		{
			return number;
		}
		std::string get_string()
		{
			return str;
		}
		abstract_syntax_tree get_function()
		{
			return function;
		}
		std::list<var> get_list()
		{
			return var_list;
		}
		std::list<var> get_hash()
		{
			return var_hash;
		}
};

class var_list_manager
{
	private:
		std::list<var> var_list;
		std::list<int> var_scope;
	public:
		var_list_manager()
		{
			var_list.clear();
			var_scope.clear();
			var null_var;
			std::string null_name_str="$";
			null_var.set_name(null_name_str);
			var_list.push_back(null_var);
			var_scope.push_back(0);
			return;
		}
		void add_var(var& tmp,int scope_depth)
		{
			for(auto i=var_list.begin();i!=var_list.end();++i)
				if(i->get_name()==tmp.get_name())
				{
					std::cout<<">>[Runtime-error] redeclaration of '"<<tmp.get_name()<<"'."<<std::endl;
					return;
				}
			var_list.push_back(tmp);
			var_scope.push_back(scope_depth);
			return;
		}
		void del_latest_var()
		{
			var_list.pop_back();
			return;
		}
		var& search_var(std::string& str)
		{
			for(auto i=var_list.begin();i!=var_list.end();++i)
				if(i->get_name()==str)
					return *i;
			std::cout<<">>[Runtime-error] '"<<str<<"' is not delclared in this scope."<<std::endl;
			return *var_list.begin();
		}
		int get_top_scope_depth()
		{
			std::list<int>::iterator i=var_scope.end();
			--i;
			return *i;
		}
};

class nasal_runtime
{
	private:
		abstract_syntax_tree root;
	public:
		nasal_runtime()
		{
			root.set_clear();
			return;
		}
		void run()
		{
			std::cout<<">>[Runtime] process begins."<<std::endl;
			int time_beg,time_end;
			time_beg=time(NULL);
			run_root(root);
			time_end=time(NULL);
			std::cout<<">>[Runtime] process exited after "<<time_beg-time_end<<" sec(s)."<<std::endl;
			return;
		}
		void set_root(abstract_syntax_tree& tree)
		{
			root.set_clear();
			root=tree;
			std::cout<<">>[Runtime] runtime got the ast-root: "<<((void *)&tree)<<"->"<<((void *)&root)<<"."<<std::endl;
			return;
		}
		void run_definition(abstract_syntax_tree& tree);
		void run_assignment(abstract_syntax_tree& tree);
		void run_loop(abstract_syntax_tree& tree);
		void run_if_else(abstract_syntax_tree& tree);
		void run_block(abstract_syntax_tree& tree);
		void run_root(abstract_syntax_tree& tree);
		var run_calculation(abstract_syntax_tree& tree);
		var run_function(abstract_syntax_tree& tree);
		var identifier_call(abstract_syntax_tree& tree);
		var scalar_call(abstract_syntax_tree& tree);
};

void nasal_runtime::run_root(abstract_syntax_tree& tree)
{
	if(!tree.get_children().empty())
	{
		for(std::list<abstract_syntax_tree>::iterator i=tree.get_children().begin();i!=tree.get_children().end();++i)
		{
			switch(i->get_type())
			{
				case __add_operator:
				case __sub_operator:
				case __mul_operator:
				case __div_operator:
				case __link_operator:
				case __and_operator:
				case __or_operator:
				case __nor_operator:
				case __cmp_equal:
				case __cmp_not_equal:
				case __cmp_less:
				case __cmp_less_or_equal:
				case __cmp_more:
				case __cmp_more_or_equal:
					run_calculation(*i);
					break;
				case __definition:
					run_definition(*i);
					break;
				case __assignment:
					run_assignment(*i);
					break;
				case __while:
				case __for:
				case __forindex:
				case __foreach:
					run_loop(*i);
					break;
				case __ifelse:
					run_if_else(*i);
					break;
				case __call_function:
					run_function(*i);
					break;
				case __id:
				case __hash_search:
				case __list_search:
					identifier_call(*i);
					break;
				case __number:
				case __string:
					scalar_call(*i);
					return;
				default:
					std::cout<<">>[Debug] error occurred."<<std::endl;
					break;
			}
		}
	}
	return;
}

void nasal_runtime::run_definition(abstract_syntax_tree& tree)
{
	var new_var;
	std::list<abstract_syntax_tree>::iterator iter=tree.get_children().begin();
	new_var.set_name(iter->get_var_name());
	++iter;
	if(iter==tree.get_children().end())
	{
		new_var.set_type(__null_type);
		
		return;
	}
	return;
}
void nasal_runtime::run_assignment(abstract_syntax_tree& tree)
{
	return;
}
void nasal_runtime::run_loop(abstract_syntax_tree& tree)
{
	return;
}
void nasal_runtime::run_if_else(abstract_syntax_tree& tree)
{
	return;
}
void nasal_runtime::run_block(abstract_syntax_tree& tree)
{
	return;
}

var nasal_runtime::run_calculation(abstract_syntax_tree& tree)
{
	var null_var;
	return null_var;
}
var nasal_runtime::run_function(abstract_syntax_tree& tree)
{
	var null_var;
	return null_var;
}
var nasal_runtime::identifier_call(abstract_syntax_tree& tree)
{
	var null_var;
	return null_var;
}
var nasal_runtime::scalar_call(abstract_syntax_tree& tree)
{
	var null_var;
	return null_var;
}
#endif
