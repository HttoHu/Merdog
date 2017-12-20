#pragma once
#include "TMemory.hpp"
#include <string>
namespace Simcc
{
	namespace Runtime
	{
		enum CountSign
		{
			Add,Mul,Div,Pp,Mm,Aeq,Meq,Deq,Seq,
			Not,And,Or,
			Ne,Eq,Gt,Ge,Lt,Le
		};
		class Object
		{
		public:
			Object() :pos(StackBlock::pc){}
			virtual std::string to_string() const { return ""; }
			virtual void Add(Object *op, Object *v) {}
			virtual void Sub(Object *op, Object *v) {}
			virtual void Mul(Object *op, Object *v) {}
			virtual void Div(Object *op, Object *v) {}
			virtual void SAdd(Object *op) {}
			virtual void SSub(Object *op) {}
			virtual void SMul(Object *op) {}
			virtual void SDiv(Object *op) {}
			virtual Object* Operator(Object *op, CountSign cs) { return false; }
			template<typename _T>
			_T& get_value()const
			{
				return *(_T*)StackBlock::find(pos);
			}
			virtual ~Object() {}
		protected:
			size_t type;
			// 0- int ,1-long 2-char 3-string 4- double 5-bool
			size_t pos;
		private:
		};
		//基本类型
		class Basic final:public Object 
		{
		public:
			Basic(int v)
			{
				StackBlock::push(v);
				type = 0;
			}
			Basic(int64_t v)
			{
				StackBlock::push(v);
				type = 1;
			}
			Basic(const char * c_str):Basic(std::string(c_str))
			{}
			Basic(const std::string & str)
			{
				StackBlock::push(str);
				type = 3;
			}
			Basic(char ch)
			{
				StackBlock::push(ch);
				type = 2;
			}
			Basic(double d) {
				StackBlock::push(d);
				type = 4;
			}
			Basic(bool b)
			{
				StackBlock::push(b);
				type = 5;
			}
			Basic(const Basic &bas)
			{
				switch (bas.type)
				{
				case 0:
					StackBlock::push(bas.get_value<int>());
					break;
				case 1:
					StackBlock::push(bas.get_value<int64_t>());
					break;
				case 2:
					StackBlock::push(bas.get_value<char>());
					break;
				case 3:
					StackBlock::push(bas.get_value<std::string>());
					break;
				case 4:
					StackBlock::push(bas.get_value<double>());
					break;
				case 5:
					StackBlock::push(bas.get_value<bool>());
					break;
				default:
					throw std::runtime_error("unkown type!");
					break;
				}
			}
			//==============init==============================
			std::string to_string()const override;
			void SAdd(Object *op)override;
			void SSub(Object *op)override;
			void SDiv(Object *op)override;
			void SMul(Object *op)override;
			void Add(Object *op, Object *v)override;
			void Sub(Object *op, Object *v)override;
			void Mul(Object *op, Object *v) override;
			void Div(Object *op, Object *v) override;
			Object* Operator(Object *op, CountSign cs)override;
			//=============================================
		private:
		};
	}
}