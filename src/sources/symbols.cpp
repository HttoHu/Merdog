#include "../includes/symbols.hpp"
#include "../includes/error.hpp"
namespace Mer
{
    namespace Symbol
    {
        void SymbleTable::new_block()
        {
            sym_tab.push_front(std::map<std::string, WordRecorder *>());
        }

        void SymbleTable::end_block()
        {
            sym_tab.pop_front();
        }

        void SymbleTable::push_word(const std::string &name, WordRecorder *sym)
        {
            if (sym_tab.front().find(name) != sym_tab.front().end())
                throw Error("id " + name + " redefined!");
            sym_tab.front().insert({name, std::move(sym)});
        }

        WordRecorder *SymbleTable::find(const std::string &id)
        {
            for (size_t i = 0; i < sym_tab.size(); i++)
            {
                auto result = sym_tab[i].find(id);
                if (result != sym_tab[i].end())
                {
                    return result->second;
                }
            }
            throw Error("undefined symbol :" + id);
        }

        // ArrayRecorder
        size_t ArrayRecorder::calc_cnt()
        {
            size_t res = 1;
            for (auto cnt : params)
                res *= cnt;
            return res;
        }
        size_t ArrayRecorder::index(const std::vector<int> &vec)
        {
            size_t count = calc_cnt();
            size_t type_len = get_type_size(type);
            if (vec.size() == 0 || vec.size() > params.size())
                throw Error("invalid array index");
            std::vector<int> suffix_times(params.size() + 1);
            suffix_times.back() = 1;
            for (int i = suffix_times.size() - 1; i >= 0; i--)
                suffix_times[i] = suffix_times[i + 1] * params[i];
            int ans = 0;
            for (int i = 0; i < vec.size(); i++)
                ans += vec[i] * suffix_times[i + 1];
            return ans * type_len;
        }
    }
}
