#pragma once
#include "statement.hpp"
#include "object.hpp"
#include <unordered_map>
namespace Merdog
{
	class Block
	{
	public:
		Block(const std::string &str,Block* par) :name(str),parent(par) {}
		void push_ins(Statement *s) { stmts.push_back(s); }
		Object::ObjectBase* find_var(Id *name)
		{
			auto val = var_stack.find(name);
			if (val != var_stack.end())
			{
				return val->second;
			}
			else if (parent != nullptr)
				return parent->find_var(name);
			throw Error(name->to_string() + "no found");
		}
		Block* create_block(const std::string &blk_name="_anonymous") {

			auto blok = new Block("_anonymous", this);
			children.push_back(blok);
			return blok;
		}
		void execute()
		{
			for (const auto &a : stmts)
				a->run();
		}
	private:
		std::string name;
		std::vector<Statement*> stmts;
		std::unordered_map<Id*,Object::ObjectBase*> var_stack;
		Block *parent;
		std::vector<Block*> children;
	};
}