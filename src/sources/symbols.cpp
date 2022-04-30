#include "../includes/symbols.hpp"
#include "../includes/error.hpp"
namespace Mer
{
	namespace Symbol
	{
		void SymbleTable::new_block()
		{
			sym_tab.push_front(std::map<std::string, WordRecorder*>());
		}

		void SymbleTable::end_block()
		{
			print();
			sym_tab.pop_front();
		}

		void SymbleTable::push_word(const std::string& name, WordRecorder* sym)
		{
			if (sym_tab.front().find(name) != sym_tab.front().end())
				throw Error("id " + name + " redefined!");
			sym_tab.front().insert({ name, std::move(sym) });
		}

		WordRecorder* SymbleTable::find(const std::string& id)
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

		void SymbleTable::print()
		{
			auto& mp = sym_tab.front();
			std::cout << "{";
			for (auto [k, v] : mp)
			{
				std::cout << "\t" << k << " : " << v->to_string() << "\n";
			}
		}

		ArrayRecorder::ArrayRecorder(type_code_index _type, const std::vector<size_t>& v, size_t _pos, bool _is_global)
			: WordRecorder(Array), type(_type), params(v), pos(_pos), is_global(_is_global)
		{
			type_len = get_type_size(type);
			suffix_prod.resize(params.size());
			suffix_prod.back() = 1;
			for (int i = suffix_prod.size() - 2; i >= 0; i--)
				suffix_prod[i] = suffix_prod[i + 1] * params[i];
		}
		// ArrayRecorder
		size_t ArrayRecorder::calc_cnt()
		{
			size_t res = 1;
			for (auto cnt : params)
				res *= cnt;
			return res;
		}
		std::string ArrayRecorder::to_string()
		{
			std::string ret = "<array : pos( " + std::to_string(pos) + "),type(" + std::to_string(type) + "),";
			for (auto v : params)
				ret += "[" + std::to_string(v) + "]";
			ret += ">";
			return ret;
		}
	}
}
