/*
* MIT License
* Copyright (c) 2019 Htto Hu
*/
#include "../include/clib/maths.hpp"
namespace Mer
{
	namespace
	{
		Mem::Object _min(const std::vector<Mem::Object>& args)
		{
			auto obj = args.front();
			double d = std::static_pointer_cast<Mem::Double>(obj->Convert(Mem::BasicType::DOUBLE))->get_value();
			double d2 = std::static_pointer_cast<Mem::Double>(args[1]->Convert(Mem::BasicType::DOUBLE))->get_value();
			double ret =std::fmin(d, d2);
			return std::make_shared<Mem::Double>(ret);
		}
		Mem::Object _sqrt(const std::vector<Mem::Object>& args)
		{
			auto obj = args.front();
			double d = std::static_pointer_cast<Mem::Double>(obj->Convert(Mem::BasicType::DOUBLE))->get_value();
			d = std::sqrt(d);
			return std::make_shared<Mem::Double>(d);
		}
		Mem::Object _mod(const std::vector<Mem::Object>& args)
		{
			auto obj = args.front();
			auto obj2 = args[1];
			auto v = std::static_pointer_cast<Mem::Int>(obj->Convert(Mem::BasicType::INT))->get_value();
			auto v2= std::static_pointer_cast<Mem::Int>(obj2->Convert(Mem::BasicType::INT))->get_value();
			return std::make_shared<Mem::Int>(v%v2);
		}
		Mem::Object _sin(const std::vector<Mem::Object>& args)
		{
			auto obj = args.front();
			double d = std::static_pointer_cast<Mem::Double>(obj->Convert(Mem::BasicType::DOUBLE))->get_value();
			d = std::sin(d);
			return std::make_shared<Mem::Double>(d);
		}
		Mem::Object _cos(const std::vector<Mem::Object>& args)
		{
			auto obj = args.front();
			double d = std::static_pointer_cast<Mem::Double>(obj->Convert(Mem::BasicType::DOUBLE))->get_value();
			d = std::cos(d);
			return std::make_shared<Mem::Double>(d);
		}
		Mem::Object _tan(const std::vector<Mem::Object>& args)
		{
			auto obj = args.front();
			double d = std::static_pointer_cast<Mem::Double>(obj->Convert(Mem::BasicType::DOUBLE))->get_value();
			d = std::tan(d);
			return std::make_shared<Mem::Double>(d);
		}
		Mem::Object _arsin(const std::vector<Mem::Object>& args)
		{
			auto obj = args.front();
			double d = std::static_pointer_cast<Mem::Double>(obj->Convert(Mem::BasicType::DOUBLE))->get_value();
			d = std::asin(d);
			return std::make_shared<Mem::Double>(d);
		}
		Mem::Object _arcos(const std::vector<Mem::Object>& args)
		{
			auto obj = args.front();
			double d = std::static_pointer_cast<Mem::Double>(obj->Convert(Mem::BasicType::DOUBLE))->get_value();
			d = std::acos(d);
			return std::make_shared<Mem::Double>(d);
		}
		Mem::Object _artan(const std::vector<Mem::Object>& args)
		{
			auto obj = args.front();
			double d = std::static_pointer_cast<Mem::Double>(obj->Convert(Mem::BasicType::DOUBLE))->get_value();
			d = std::atan(d);
			return std::make_shared<Mem::Double>(d);
		}
		Mem::Object _abs(const std::vector<Mem::Object>& args)
		{
			auto obj = args.front();
			double d = std::static_pointer_cast<Mem::Double>(obj->Convert(Mem::BasicType::DOUBLE))->get_value();
			d = std::abs(d);
			return std::make_shared<Mem::Double>(d);
		}
		Mem::Object _pow(const std::vector<Mem::Object>& args)
		{
			auto obj = args.front();
			auto obj2 = args[1];
			auto v = std::static_pointer_cast<Mem::Double>(obj)->get_value();
			auto v2 = std::static_pointer_cast<Mem::Double>(obj2)->get_value();
			return std::make_shared<Mem::Double>(pow(v,v2));
		}
	}
	Namespace* maths;
	void set_maths()
	{
		Mer::SystemFunction* min = new SystemFunction(Mem::DOUBLE, _min);
		Mer::SystemFunction* sin = new SystemFunction(Mem::DOUBLE, _sin);
		Mer::SystemFunction* cos = new SystemFunction(Mem::DOUBLE, _cos);
		Mer::SystemFunction* tan = new SystemFunction(Mem::DOUBLE, _tan);
		Mer::SystemFunction* arsin = new SystemFunction(Mem::DOUBLE, _arsin);
		Mer::SystemFunction* arcos = new SystemFunction(Mem::DOUBLE, _arcos);
		Mer::SystemFunction* artan = new SystemFunction(Mem::DOUBLE, _artan);
		Mer::SystemFunction* mod = new SystemFunction(Mem::INT, _mod);
		Mer::SystemFunction* sqrt = new SystemFunction(Mem::DOUBLE, _sqrt);
		Mer::SystemFunction* abs = new SystemFunction(Mem::DOUBLE, _abs);
		Mer::SystemFunction* my_pow = new SystemFunction(Mem::DOUBLE, _pow);
		maths = root_namespace;
		min->set_param_types({ Mem::DOUBLE,Mem::DOUBLE });
		sqrt->set_param_types({ Mem::DOUBLE });
		mod->set_param_types({ Mem::INT ,Mem::INT });
		tan->set_param_types({ Mem::DOUBLE });
		sin->set_param_types({ Mem::DOUBLE });
		cos->set_param_types({ Mem::DOUBLE });
		arsin->set_param_types({ Mem::DOUBLE });
		arcos->set_param_types({ Mem::DOUBLE });
		artan->set_param_types({ Mem::DOUBLE });
		abs->set_param_types({ Mem::DOUBLE });
		my_pow->set_param_types({ Mem::DOUBLE,Mem::DOUBLE });
		maths->set_new_func("min2", min);
		maths->set_new_func("sqrt",sqrt);
		maths->set_new_func("mod", mod);
		maths->set_new_func("tan", tan);
		maths->set_new_func("sin", sin);
		maths->set_new_func("cos",  cos);
		maths->set_new_func("arsin", arsin);
		maths->set_new_func("arcos", arcos);
		maths->set_new_func("artan",  artan);
		maths->set_new_func("abs", abs);
		maths->set_new_func("pow", my_pow);
	}
}