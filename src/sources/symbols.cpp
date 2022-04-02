#include "../includes/symbols.hpp"
#include "../includes/error.hpp"
namespace Mer {
	namespace Symbol {
		void SymbleTable::new_block()
		{
			sym_tab.push_front(std::map<std::string, WordRecorder*>());
		}

		void SymbleTable::end_block()
		{
			sym_tab.pop_front();
		}

		void SymbleTable::push_word(const std::string& name, WordRecorder* sym)
		{
			if (sym_tab.front().find(name) != sym_tab.front().end())
				throw Error("id " + name + " redefined!");
			sym_tab.front().insert({ name,std::move(sym) });
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

	}
}

