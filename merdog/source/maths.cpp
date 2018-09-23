#include "../include/clib/maths.hpp"
namespace Mer
{
	namespace
	{
		Mem::Object _min(std::vector<Mem::Object>& args)
		{
			auto obj = args.front();
			double d = std::static_pointer_cast<Mem::Double>(obj->Convert(Mem::BasicType::DOUBLE))->get_value();
			double d2 = std::static_pointer_cast<Mem::Double>(args[1]->Convert(Mem::BasicType::DOUBLE))->get_value();
			double ret = fminf(d, d2);
			return std::make_shared<Mem::Double>(ret);
		}
		Mem::Object _sqrt(std::vector<Mem::Object>& args)
		{
			auto obj = args.front();
			double d = std::static_pointer_cast<Mem::Double>(obj->Convert(Mem::BasicType::DOUBLE))->get_value();
			d = std::sqrt(d);
			return std::make_shared<Mem::Double>(d);
		}
		Mem::Object _mod(std::vector<Mem::Object>& args)
		{
			auto obj = args.front();
			auto obj2 = args[1];
			auto v = std::static_pointer_cast<Mem::Int>(obj->Convert(Mem::BasicType::INT))->get_value();
			auto v2= std::static_pointer_cast<Mem::Int>(obj2->Convert(Mem::BasicType::INT))->get_value();
			return std::make_shared<Mem::Int>(v%v2);
		}
		Mem::Object _sin(std::vector<Mem::Object>& args)
		{
			auto obj = args.front();
			double d = std::static_pointer_cast<Mem::Double>(obj->Convert(Mem::BasicType::DOUBLE))->get_value();
			d = std::sin(d);
			return std::make_shared<Mem::Double>(d);
		}
		Mem::Object _cos(std::vector<Mem::Object>& args)
		{
			auto obj = args.front();
			double d = std::static_pointer_cast<Mem::Double>(obj->Convert(Mem::BasicType::DOUBLE))->get_value();
			d = std::cos(d);
			return std::make_shared<Mem::Double>(d);
		}
		Mem::Object _tan(std::vector<Mem::Object>& args)
		{
			auto obj = args.front();
			double d = std::static_pointer_cast<Mem::Double>(obj->Convert(Mem::BasicType::DOUBLE))->get_value();
			d = std::tan(d);
			return std::make_shared<Mem::Double>(d);
		}
		Mem::Object _arsin(std::vector<Mem::Object>& args)
		{
			auto obj = args.front();
			double d = std::static_pointer_cast<Mem::Double>(obj->Convert(Mem::BasicType::DOUBLE))->get_value();
			d = std::asin(d);
			return std::make_shared<Mem::Double>(d);
		}
		Mem::Object _arcos(std::vector<Mem::Object>& args)
		{
			auto obj = args.front();
			double d = std::static_pointer_cast<Mem::Double>(obj->Convert(Mem::BasicType::DOUBLE))->get_value();
			d = std::acos(d);
			return std::make_shared<Mem::Double>(d);
		}
		Mem::Object _artan(std::vector<Mem::Object>& args)
		{
			auto obj = args.front();
			double d = std::static_pointer_cast<Mem::Double>(obj->Convert(Mem::BasicType::DOUBLE))->get_value();
			d = std::atan(d);
			return std::make_shared<Mem::Double>(d);
		}
		Mem::Object _abs(std::vector<Mem::Object>& args)
		{
			auto obj = args.front();
			double d = std::static_pointer_cast<Mem::Double>(obj->Convert(Mem::BasicType::DOUBLE))->get_value();
			d = std::abs(d);
			return std::make_shared<Mem::Double>(d);
		}
	}
	Namespace *maths = new Namespace(nullptr);
	Mer::SystemFunction *min = new SystemFunction(Mem::DOUBLE, _min);
	Mer::SystemFunction *sin = new SystemFunction(Mem::DOUBLE, _sin);
	Mer::SystemFunction *cos = new SystemFunction(Mem::DOUBLE, _cos);
	Mer::SystemFunction *tan = new SystemFunction(Mem::DOUBLE, _tan);
	Mer::SystemFunction *arsin = new SystemFunction(Mem::DOUBLE, _arsin);
	Mer::SystemFunction *arcos = new SystemFunction(Mem::DOUBLE, _arcos);
	Mer::SystemFunction *artan = new SystemFunction(Mem::DOUBLE, _artan);
	Mer::SystemFunction *mod = new SystemFunction(Mem::INT, _mod);
	Mer::SystemFunction *sqrt = new SystemFunction(Mem::DOUBLE, _sqrt);
	Mer::SystemFunction *abs = new SystemFunction(Mem::DOUBLE, _abs);
	void Mer::set_maths()
	{
		maths = root_namespace;
		min->set_param_types({ Mem::DOUBLE });
		sqrt->set_param_types({ Mem::DOUBLE });
		mod->set_param_types({ Mem::DOUBLE });
		tan->set_param_types({ Mem::DOUBLE });
		sin->set_param_types({ Mem::DOUBLE });
		cos->set_param_types({ Mem::DOUBLE });
		arsin->set_param_types({ Mem::DOUBLE });
		arcos->set_param_types({ Mem::DOUBLE });
		artan->set_param_types({ Mem::DOUBLE });
		abs->set_param_types({ Mem::DOUBLE });
		//mstd->children.insert({ "maths", maths });
		maths->set_new_func("min2", Mem::DOUBLE, min);
		maths->set_new_func("sqrt",Mem::DOUBLE,sqrt);
		maths->set_new_func("mod", Mem::INT, mod);
		maths->set_new_func("tan", Mem::DOUBLE,tan);
		maths->set_new_func("sin", Mem::DOUBLE, sin);
		maths->set_new_func("cos", Mem::DOUBLE, cos);
		maths->set_new_func("arsin", Mem::DOUBLE, arsin);
		maths->set_new_func("arcos", Mem::DOUBLE, arcos);
		maths->set_new_func("artan", Mem::DOUBLE, artan);
		maths->set_new_func("abs", Mem::DOUBLE, abs);
	}
}