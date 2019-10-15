/*
	By Htto, simplified boost::any;
	===========================================
	Boost Software License - Version 1.0 - August 17th, 2003
	Permission is hereby granted, free of charge, to any person or organization
	obtaining a copy of the software and accompanying documentation covered by
	this license (the "Software") to use, reproduce, display, distribute,
	execute, and transmit the Software, and to prepare derivative works of the
	Software, and to permit third-parties to whom the Software is furnished to
	do so, all subject to the following:
	The copyright notices in the Software and this entire statement, including
	the above license grant, this restriction and the following disclaimer,
	must be included in all copies of the Software, in whole or in part, and
	all derivative works of the Software, unless such copies or derivative
	works are solely in the form of machine-executable object code generated by
	a source language processor.
	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
	SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE=
	FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
	ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
	DEALINGS IN THE SOFTWARE.
*/
#pragma once
#include <string>
#include <iostream>
namespace mer
{
#define TYPE_CHECK
	//#undef TYPE_CHECK
	class placeholder
	{
	public:
		virtual ~placeholder() {}
		virtual placeholder* clone() = 0;
		virtual const std::type_info& type()noexcept = 0;
	};
	template<typename ValueType>
	class holder final :public placeholder
	{
	public:
		holder(const ValueType vt) :value(vt) {}
		placeholder* clone()override
		{
			return new holder<ValueType>(value);
		}
		virtual const std::type_info& type()noexcept override
		{
			return typeid(ValueType);
		}
		ValueType value;
	};

	class any
	{
	public:
		any() :content(nullptr) {}
		template<typename ValueType>
		any(const ValueType& value)
			: content(new holder<ValueType>(value)) {
		}
		any(const char* str) :any(std::string(str)) {}
		any(const any& other)
			: content(other.content ? other.content->clone() : 0) {}
		template <class ValueType>
		any& operator=(ValueType&& rhs)
		{
			any(static_cast<ValueType&&>(rhs)).swap(*this);
			return *this;
		}
		any& swap(any& rhs) noexcept
		{
			std::swap(content, rhs.content);
			return *this;
		}
		any& operator=(const any& rhs)
		{
			any(rhs).swap(*this);
			return *this;
		}

		// move assignment
		any& operator=(any&& rhs) noexcept
		{
			rhs.swap(*this);
			any().swap(rhs);
			return *this;
		}
	private:
		placeholder* content;
		template<typename _Ty>
		friend _Ty any_cast(const any& a);
	};
	// when any_cast fail, it will throw the bad_cast
	class  bad_any_cast :public std::bad_cast
	{
	public:
		virtual const char* what() const override
		{
			return "bad mer::any_cast";
		}
	};
	template<typename _Ty>
	_Ty any_cast(const any& a)
	{
#ifdef TYPE_CHECK
		if (typeid(_Ty) != a.content->type())
			throw mer::bad_any_cast();
#endif
		return static_cast<holder<_Ty>*>(a.content)->value;
	}
}