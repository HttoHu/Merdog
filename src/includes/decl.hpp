#pragma once
#include "./parser_node.hpp"
#include "./defs.hpp"
#include "./lexer.hpp"
#include <vector>

namespace Mer
{
    class NamePart
    {
    public:
        NamePart();
        size_t get_count() { return count; }
        bool is_array() { return arr; }
        bool is_auto_array() { return auto_array; }
        bool is_pointer() { return pointer; }
        Token *get_id() { return id; }
        size_t get_array_element_cnt() {
            size_t prod = 1;
            for (auto num : array_indexs)
                prod *= num;
            return prod;
        }
        std::vector<size_t> array_indexs;

    private:
        Token *id;
        // int a[]={1,2,3};
        bool auto_array = false;
        bool arr = false;
        bool pointer = false;
        size_t count = 1;
    };
    
    class VarDecl : public ParserNode
    {
    public:
        using NameT = std::vector<std::string>;
        using PartT = std::vector<std::pair<::size_t, ParserNode *>>;
        // length ,node
        VarDecl(const NameT &_ids, const PartT &parts, size_t *_base_ptr, ::size_t _pos);
        void execute(char *) override;

        std::string to_string() const override;
        size_t *base_ptr;
        size_t pos;

    private:
        std::vector<std::pair<size_t, ParserNode *>> values;
        std::vector<std::string> ids;
    };
    namespace Parser
    {
        ParserNode *parse_var_decl();
        void parse_const_decl();
        ParserNode *statement();
    }
    namespace Utils
    {
        int_default get_int_const_expr_value();
    }
}