#pragma once
#include <string>
#include <vector>
#include <map>
#include "../../Lexer/HPP/Token.hpp"
#include "Object.hpp"
namespace Simcc
{
	namespace Runtime
	{
		// ��д�������, ��һ�¹��ƴ������Ҫ����. �����Ժ�����ܴ�����������ܵ�����. ���ҽ��﷨�������ƺ��ٸ�.
		// ����ֻҪ���ʽ�ĸľͺ���.
		class Stack{
		public:
			Stack()
			{
				newBlock();
			}
			void newBlock();
			void endBlock();
			ObjectBase* find_variable(Lexer::Token* );
			void push(Lexer::Token* , ObjectBase *obj);
			void debug();
		private:
			std::list<std::map<Lexer::Token*,ObjectBase*>> stack_memory;
		};
	}
}