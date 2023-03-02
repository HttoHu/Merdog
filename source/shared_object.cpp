#include <iostream>
#include "../include/shared_object.hpp"
#include "../include/namespace.hpp"
#include <dlfcn.h>
namespace Mer
{
    void SharedObjEnv::load_so(const std::string &libname,const std::string &file_name)
    {
        auto handler = dlopen(file_name.c_str(), RTLD_LAZY);
        if (!handler)
            throw Error("open shared lib " + libname + " failed! : " + std::string(dlerror()));
        so_handler_tab.insert({libname, handler});
    }

    _intern_func SharedObjEnv::find_so_func(const std::string &so_name, const std::string &func_name)
    {
        auto it = so_handler_tab.find(so_name);
        if (it == so_handler_tab.end())
            throw Error("shared lib " + so_name + " not found!");
        _mer_func_type *func_ptr = (_mer_func_type *)dlsym(it->second, func_name.c_str());
        if (!func_ptr)
            throw Mer::Error("unknown symbol " + func_name);
        return std::function(func_ptr);
    }

    void SharedObjEnv::parse_load()
    {
        token_stream.match(LOAD_SHARED);
        std::string lib_name = Id::get_value(token_stream.this_token());
        token_stream.next();
        token_stream.match(ASSIGN);
        std::string file_name = String::get_value(token_stream.this_token());
        token_stream.next();
        load_so(lib_name,file_name);
    }
    void SharedObjEnv::parse_shared()
    {
        token_stream.match(SHARED);
        if (token_stream.this_tag() == FUNCTION)
            parse_shared_func();
        else
        {
            throw Mer::Error("unknown shared type!");
        }
    }

    void SharedObjEnv::parse_shared_func()
    {
        token_stream.match(FUNCTION);
        // func type
        type_code_index rtype = Mem::get_type_code();
        if (token_stream.this_tag() == MUL)
        {
            token_stream.next();
            rtype++;
        }
        // func name
        std::string func_name = Id::get_value(token_stream.this_token());
        token_stream.next();
        auto params_types = Parser::build_param_feature();

        token_stream.match(ASSIGN);
        std::string lib_name = Id::get_value(token_stream.this_token());
        token_stream.next();
        token_stream.match(DOT);
        std::string lib_sym_name = Id::get_value(token_stream.this_token());
        token_stream.next();
        auto raw_func = find_so_func(lib_name, lib_sym_name);
        // build function object
        SystemFunction *func = new SystemFunction(rtype, raw_func);
        func->set_param_types(params_types);
        // update symbol table
        this_namespace->set_new_func(func_name, func);
    }
}