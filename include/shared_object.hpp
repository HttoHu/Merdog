#pragma once
#define UNIX_LIKE
#ifdef UNIX_LIKE
#include <map>
#include <string>
#include <dlfcn.h>
#include "./basic_objects.hpp"
#include "./function.hpp"
namespace Mer
{
    class SharedObjEnv
    {
    public:
        void load_so(const std::string & lib_name,const std::string &so_name);
        _intern_func find_so_func(const std::string &so_name, const std::string &func_name);
        void parse_load();
        void parse_shared();
        ~SharedObjEnv()
        {
            for (auto [name, handler] : so_handler_tab)
                dlclose(handler);
        }

    public:
        std::map<std::string, void *> so_handler_tab;
        void parse_shared_func();
        void parse_shared_struct();
    };

}

#endif

#ifdef WINDOWS_LIKE
#endif