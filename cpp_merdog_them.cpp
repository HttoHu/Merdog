// haha Just a toy.
#define function
#include <iostream>
#include <string>
class merdog_io
{
public:
	template<typename T>
	void cout(T t)
	{
		std::cout << t;
	}
	template<typename T, typename... Args>
	void cout(T t, Args... args)
	{
		cout(t);
		cout(args...);
	}
	int input_int()
	{
		int ret;
		std::cin >> ret;
		return ret;
	}
	double input_real()
	{
		double ret;
		std::cin >> ret;
		return ret;
	}
	char input_char()
	{
		char ret;
		std::cin >> ret;
		return ret;
	}
	std::string input_string()
	{
		std::string str;
		std::cin >> str;
		return str;
	}
}mstd;
#define cast static_cast
#define real double
#define program 
#define main int main()
template<size_t count,typename ArrType>
int msizeof(ArrType (&arr)[count])
{
	return count;
}
#define sizeof msizeof
#define vector std::vector
#define deque std::deque
#define string std::string
#include <vector>
#include <deque>
#define std mstd
//=====================COPY YOUR MERDOG PROGRAM HERE AND BUILD IT WITH CPP COMPILER=================
