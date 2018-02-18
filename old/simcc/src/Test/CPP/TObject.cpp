#include "../HPP/TObject.hpp"

std::string Simcc::Runtime::Basic::to_string() const
{
	switch (type)
	{
	case 0:
		return std::to_string(get_value<int>());
	case 1:
		return std::to_string(get_value<int64_t>());
	case 2:
		return std::string(1, get_value<char>());
	case 3:
		return get_value<std::string>();
	case 4:
		return std::to_string(get_value<double>());
	case 5:
		if (get_value<bool>())
			return "true";
		return "false";
	default:
		throw std::runtime_error("unkown type!");
	}
}

void Simcc::Runtime::Basic::SAdd(Object * op)
{
	switch (type)
	{
	case 0:
		get_value<int>() += op->get_value<int>();
		break;
	case 1:
		get_value<int64_t>() += op->get_value<int64_t>();
		break;
	case 2:
		get_value<char>() += op->get_value<char>();
		break;
	case 3:
		get_value<std::string>() += op->get_value<std::string>();
		break;
	case 4:
		get_value<double>() += op->get_value<double>();
		break;
	case 5:
		throw std::runtime_error("type not matched!");
	default:
		throw std::runtime_error("unkown type!");
	}
}

void Simcc::Runtime::Basic::SSub(Object * op)
{
	switch (type)
	{
	case 0:
		get_value<int>() -= op->get_value<int>();
		break;
	case 1:
		get_value<int64_t>() -= op->get_value<int64_t>();
		break;
	case 2:
		get_value<char>() -= op->get_value<char>();
		break;
	case 3:
		throw std::runtime_error("type not matched!");
		break;
	case 4:
		get_value<double>() -= op->get_value<double>();
		break;
	case 5:
		throw std::runtime_error("type not matched!");
	default:
		throw std::runtime_error("unkown type!");
	}
}

void Simcc::Runtime::Basic::SMul(Object * op)
{
	switch (type)
	{
	case 0:
		get_value<int>() *= op->get_value<int>();
		break;
	case 1:
		get_value<int64_t>() *= op->get_value<int64_t>();
		break;
	case 2:
		get_value<char>() *= op->get_value<char>();
		break;
	case 3:
		throw std::runtime_error("type not matched!");
		break;
	case 4:
		get_value<double>() *= op->get_value<double>();
		break;
	case 5:
		throw std::runtime_error("type not matched!");
	default:
		throw std::runtime_error("unkown type!");
	}
}

void Simcc::Runtime::Basic::Add(Object * op, Object * v)
{
	switch (type)
	{
	case 0:
		v->get_value<int>() =get_value<int>()+ op->get_value<int>();
		break;
	case 1:
		v->get_value<int64_t>() =get_value<int64_t>()+ op->get_value<int64_t>();
		break;
	case 2:
		v->get_value<char>() =get_value<char>()+ op->get_value<char>();
		break;
	case 3:
		v->get_value<std::string>() =get_value<std::string>()+ op->get_value<std::string>();
		break;
	case 4:
		v->get_value<double>() =get_value<double>()+ op->get_value<double>();
		break;
	case 5:
		throw std::runtime_error("type not matched!");
	default:
		throw std::runtime_error("unkown type!");
	}
}

void Simcc::Runtime::Basic::SDiv(Object * op)
{
	switch (type)
	{
	case 0:
		get_value<int>() /= op->get_value<int>();
		break;
	case 1:
		get_value<int64_t>() /= op->get_value<int64_t>();
		break;
	case 2:
		get_value<char>() /= op->get_value<char>();
		break;
	case 3:
		throw std::runtime_error("type not matched!");
		break;
	case 4:
		get_value<double>() /= op->get_value<double>();
		break;
	case 5:
		throw std::runtime_error("type not matched!");
	default:
		throw std::runtime_error("unkown type!");
	}
}

void Simcc::Runtime::Basic::Sub(Object * op, Object * v)
{
	switch (type)
	{
	case 0:
		v->get_value<int>() = get_value<int>() - op->get_value<int>();
		break;
	case 1:
		v->get_value<int64_t>() = get_value<int64_t>() - op->get_value<int64_t>();
		break;
	case 2:
		v->get_value<char>() = get_value<char>() - op->get_value<char>();
		break;
	case 3:
		throw std::runtime_error("type not matched!");
		break;
	case 4:
		v->get_value<double>() = get_value<double>() - op->get_value<double>();
		break;
	case 5:
		throw std::runtime_error("type not matched!");
	default:
		throw std::runtime_error("unkown type!");
	}
}

void Simcc::Runtime::Basic::Div(Object * op, Object * v)
{
	switch (type)
	{
	case 0:
		v->get_value<int>() = get_value<int>() / op->get_value<int>();
		break;
	case 1:
		v->get_value<int64_t>() = get_value<int64_t>() / op->get_value<int64_t>();
		break;
	case 2:
		v->get_value<char>() = get_value<char>() / op->get_value<char>();
		break;
	case 3:
		throw std::runtime_error("type not matched!");
		break;
	case 4:
		v->get_value<double>() = get_value<double>() / op->get_value<double>();
		break;
	case 5:
		throw std::runtime_error("type not matched!");
	default:
		throw std::runtime_error("unkown type!");
	}
}
Simcc::Runtime::Object* Simcc::Runtime::Basic::Operator(Object * op, CountSign cs)
{
	Object* ret;
	switch (cs)
	{
	case Simcc::Runtime::Add:
	{
		switch (type)
		{
		case 0:
			ret=new Basic(get_value<int>() + op->get_value<int>());
			break;
		case 1:
			ret = new Basic(get_value<int64_t>() + op->get_value<int64_t>());
			break;
		case 2:
			ret = new Basic(get_value<char>() + op->get_value<char>());
			break;
		case 3:
			ret = new Basic(get_value<std::string>() + op->get_value<std::string>());
			break;
		case 4:
			ret = new Basic(get_value<double>() + op->get_value<double>());
			break;
		case 5:
			throw std::runtime_error("type not matched!");
		default:
			throw std::runtime_error("unkown type!");
		}
		return ret;
	}
	case Simcc::Runtime::Mul:
	{
		switch (type)
		{
		case 0:
			ret = new Basic(get_value<int>() * op->get_value<int>());
			break;
		case 1:
			ret = new Basic(get_value<int64_t>() * op->get_value<int64_t>());
			break;
		case 2:
			ret = new Basic(get_value<char>() * op->get_value<char>()));
			break;
		case 3:
			throw std::runtime_error("type not matched!");
			break;
		case 4:
			ret = new Basic(get_value<double>() * op->get_value<double>());
			break;
		case 5:
			throw std::runtime_error("type not matched!");
		default:
			throw std::runtime_error("unkown type!");
		}
		return ret;
	}
	case Simcc::Runtime::Div:
	{
		switch (type)
		{
		case 0:
			ret = new Basic(get_value<int>() / op->get_value<int>());
			break;
		case 1:
			ret = new Basic(get_value<int64_t>() / op->get_value<int64_t>());
			break;
		case 2:
			ret = new Basic(get_value<char>() / op->get_value<char>());
			break;
		case 3:
			throw std::runtime_error("type not matched!");
			break;
		case 4:
			ret = new Basic(get_value<double>() / op->get_value<double>());
			break;
		case 5:
			throw std::runtime_error("type not matched!");
		default:
			throw std::runtime_error("unkown type!");
		}
		return ret;
	}
	case Simcc::Runtime::Aeq:
	{
		switch (type)
		{
		case 0:
			get_value<int>() += op->get_value<int>();
			break;
		case 1:
			 get_value<int64_t>() += op->get_value<int64_t>();
			break;
		case 2:
			 get_value<char>() += op->get_value<char>();
			break;
		case 3:
			get_value<std::string>() += op->get_value<std::string>();
			break;
		case 4:
			 get_value<double>() += op->get_value<double>();
			break;
		case 5:
			get_value<bool>() += op->get_value<bool>();
			break;
		default:
			throw std::runtime_error("unkown type!");
		}
		return this;
	}
	case Simcc::Runtime::Meq:
	{
		switch (type)
		{
		case 0:
			get_value<int>() *= op->get_value<int>();
			break;
		case 1:
			get_value<int64_t>() *= op->get_value<int64_t>();
			break;
		case 2:
			get_value<char>() *= op->get_value<char>();
			break;
		case 3:
			throw std::runtime_error(" type error!");
			break;
		case 4:
			get_value<double>() *= op->get_value<double>();
			break;
		case 5:
			get_value<bool>() += op->get_value<bool>();
			break;
		default:
			throw std::runtime_error("unkown type!");
		}
		return this;
	}
	case Simcc::Runtime::Deq:
	{
		switch (type)
		{
		case 0:
			get_value<int>() /= op->get_value<int>();
			break;
		case 1:
			get_value<int64_t>() /= op->get_value<int64_t>();
			break;
		case 2:
			get_value<char>() /= op->get_value<char>();
			break;
		case 3:
			throw std::runtime_error(" type error!");
			break;
		case 4:
			get_value<double>() /= op->get_value<double>();
			break;
		case 5:
			get_value<bool>() /= op->get_value<bool>();
			break;
		default:
			throw std::runtime_error("unkown type!");
		}
		return this;
	}
	case Simcc::Runtime::Seq:
	{
		switch (type)
		{
		case 0:
			get_value<int>() -= op->get_value<int>();
			break;
		case 1:
			get_value<int64_t>() -= op->get_value<int64_t>();
			break;
		case 2:
			get_value<char>() -= op->get_value<char>();
			break;
		case 3:
			throw std::runtime_error(" type error!");
			break;
		case 4:
			get_value<double>() -= op->get_value<double>();
			break;
		case 5:
			get_value<bool>() -= op->get_value<bool>();
			break;
		default:
			throw std::runtime_error("unkown type!");
		}
		return this;
	}
	case Simcc::Runtime::Not:
	{
		get_value<bool>() = !get_value<bool>();
		return this;
	}
	case Simcc::Runtime::And:
	{
		ret = new Basic(get_value<bool>() && op->get_value<bool>());
		return ret;
	}

	case Simcc::Runtime::Or:
	{
		ret = new Basic(get_value<bool>() || op->get_value<bool>());
		return ret;
	}
	case Simcc::Runtime::Ne:
	{
		ret = new Basic(get_value<bool>() != op->get_value<bool>());
		return ret;
	}
	case Simcc::Runtime::Eq:
	{
		ret = new Basic(get_value<bool>() == op->get_value<bool>());
		return ret;
	}
	case Simcc::Runtime::Gt:
	{
		ret = new Basic(get_value<bool>() > op->get_value<bool>());
		return ret;
	}
	case Simcc::Runtime::Ge:
	{
		ret = new Basic(get_value<bool>() >= op->get_value<bool>());
		return ret;
	}
	case Simcc::Runtime::Lt:
	{
		ret = new Basic(get_value<bool>() <op->get_value<bool>());
		return ret;
	}
	case Simcc::Runtime::Le:
	{
		ret = new Basic(get_value<bool>() <= op->get_value<bool>());
		return ret;
	}
	default:
		throw std::runtime_error("type not matched");
		break;
	}
}

void Simcc::Runtime::Basic::Mul(Object * op, Object * v)
{
	switch (type)
	{
	case 0:
		v->get_value<int>() = get_value<int>() * op->get_value<int>();
		break;
	case 1:
		v->get_value<int64_t>() = get_value<int64_t>() * op->get_value<int64_t>();
		break;
	case 2:
		v->get_value<char>() = get_value<char>() * op->get_value<char>();
		break;
	case 3:
		throw std::runtime_error("type not matched!");
		break;
	case 4:
		v->get_value<double>() = get_value<double>() * op->get_value<double>();
		break;
	case 5:
		throw std::runtime_error("type not matched!");
	default:
		throw std::runtime_error("unkown type!");
	}
}