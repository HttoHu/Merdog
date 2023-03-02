#pragma once
#include <memory>
#include <string>
#include <vector>
#include "./any.h"
namespace Mer
{
    class Error
    {
    public:
        Error(const std::string &str) : ems(str) {}
        virtual std::string what()
        {
            return ems + "\n";
        }

    protected:
        std::string ems;
    };
    using type_code_index = size_t;
    namespace Mem
    {
        class Type;
        class Value;
        enum BasicType
        {
            ARRAY = -2,
            NDEF = -1,
            BVOID = 0,
            INT = 1,
            DOUBLE = 3,
            STRING = 5,
            BOOL = 7,
            CHAR = 9,
            INIT_LIST = 11,
        };
        using Object = std::shared_ptr<Value>;
        void swap(Object rhs, Object lhs);
        class Value
        {
        public:
            virtual Object clone() const { return nullptr; }
            virtual std::string to_string() const { return ""; }
            virtual type_code_index get_type() const
            {
                return BasicType::NDEF;
            }
            virtual Object operator=(Object v)
            {
                return nullptr;
            }
            virtual Object operator+=(Object v)
            {
                throw Error("+=: syntax error");
            }
            virtual Object operator-=(Object v)
            {
                throw Error("-=: syntax error");
            }
            virtual Object operator*=(Object v)
            {
                throw Error("*=: syntax error");
            }
            virtual Object operator/=(Object v)
            {
                throw Error("/=: syntax error");
            }
            virtual Object operator+(Object v) { throw Error("+: syntax error"); }
            virtual Object operator-(Object v) { throw Error("-: syntax error"); }
            virtual Object operator*(Object v) { throw Error("* syntax error"); }
            virtual Object operator/(Object v) { throw Error("/: syntax error"); }
            virtual Object Convert(type_code_index type) { throw Error("call convert on Value!"); }
            virtual Object get_negation()
            {
                throw Error("get_negation: syntax error");
            }
            virtual Object operator>(Object v) { throw Error(">: syntax error"); }
            virtual Object operator<(Object v) { throw Error("<: syntax error"); }
            virtual Object operator>=(Object v) { throw Error(">=: syntax error"); }
            virtual Object operator<=(Object v) { throw Error("<=: syntax error"); }
            virtual Object operator!=(Object v) { throw Error("!=: syntax error"); }
            virtual Object operator==(Object v) { throw Error("==: syntax error"); }
            virtual Object operator[](Object v) { throw Error("[]: syntax error"); }
            virtual Object operator&&(Object v) { throw Error("&&: syntax error"); }
            virtual Object operator||(Object v) { throw Error("OR: syntax error"); }
            virtual ~Value() {}

        private:
        };

        Mem::Object create_var_t(type_code_index type);
        // every obj extends Value, and override some common operator
        // size_t get_type() to get the obj's type
        // clone , copy the obj and return it;
        // Convert(BasicType), to convert the Obj to another compatible type obj.
        class Int;
        class Bool : public Value
        {
        public:
            Bool(bool b) : value(b) {}
            Object operator=(Object v) override
            {
                auto tmp = v;
                value = std::static_pointer_cast<Bool>(v)->value;
                return tmp;
            }
            std::string to_string() const override
            {
                if (value)
                    return "true";
                return "false";
            }
            type_code_index get_type() const override
            {
                return BasicType::BOOL;
            }
            Object Convert(type_code_index type) override
            {
            }
            Object get_negation() override
            {
                return std::make_shared<Mem::Bool>(!value);
            }
            Object operator==(Object v) override
            {
                return std::make_shared<Mem::Bool>(value == std::static_pointer_cast<Bool>(v)->value);
            }
            Object operator!=(Object v) override
            {
                return std::make_shared<Mem::Bool>(value != std::static_pointer_cast<Bool>(v)->value);
            }
            Object operator&&(Object v) override
            {
                return std::make_shared<Mem::Bool>(value && std::static_pointer_cast<Bool>(v)->value);
            }
            Object operator||(Object v) override
            {
                return std::make_shared<Mem::Bool>(value || std::static_pointer_cast<Bool>(v)->value);
            }
            Object clone() const override
            {
                return std::make_shared<Mem::Bool>(value);
            }
            bool _value() { return value; }

        private:
            bool value;
        };
        class Int : public Value
        {
        public:
            Int(int v) : value(v) {}
            std::string to_string() const override
            {
                return std::to_string(value);
            }
            type_code_index get_type() const override
            {
                return BasicType::INT;
            }
            Object operator=(Object v) override
            {
                value = std::static_pointer_cast<Int>(v)->value;
                return clone();
            }
            Object operator+=(Object v) override
            {
                return std::make_shared<Int>(value += std::static_pointer_cast<Int>(v)->value);
            }
            Object operator-=(Object v) override
            {
                return std::make_shared<Int>(value -= std::static_pointer_cast<Int>(v)->value);
            }
            Object operator*=(Object v) override
            {
                return std::make_shared<Int>(value *= std::static_pointer_cast<Int>(v)->value);
            }
            Object operator/=(Object v) override
            {
                return std::make_shared<Int>(value /= std::static_pointer_cast<Int>(v)->value);
            }
            Object operator+(Object v) override
            {
                return std::make_shared<Int>(value + std::static_pointer_cast<Int>(v)->value);
            }
            Object operator-(Object v) override
            {
                return std::make_shared<Int>(value -
                                             std::static_pointer_cast<Int>(v)->value);
            }
            Object operator*(Object v) override
            {
                return std::make_shared<Int>(value *
                                             std::static_pointer_cast<Int>(v)->value);
            }
            Object operator/(Object v) override
            {
                return std::make_shared<Int>(value /
                                             std::static_pointer_cast<Int>(v)->value);
            }
            Object operator>(Object v) override
            {
                return std::make_shared<Bool>(value > std::static_pointer_cast<Int>(v)->value);
            }
            Object operator>=(Object v) override
            {
                return std::make_shared<Bool>(value >= std::static_pointer_cast<Int>(v)->value);
            }
            Object operator<(Object v) override
            {
                return std::make_shared<Bool>(value < std::static_pointer_cast<Int>(v)->value);
            }
            Object operator<=(Object v) override
            {
                return std::make_shared<Bool>(value <= std::static_pointer_cast<Int>(v)->value);
            }
            Object operator==(Object v) override
            {
                return std::make_shared<Bool>(value == std::static_pointer_cast<Int>(v)->value);
            }
            Object operator!=(Object v) override
            {
                return std::make_shared<Bool>(value != std::static_pointer_cast<Int>(v)->value);
            }
            Object clone() const override
            {
                return std::make_shared<Int>(value);
            }
            Object get_negation() override
            {
                return std::make_shared<Int>(-value);
            }
            int get_value()
            {
                return value;
            }
            static int get_val(Mem::Object obj)
            {
                return std::static_pointer_cast<Mem::Int>(obj)->get_value();
            }
            Object Convert(type_code_index type) override
            {
            }
            Object operator[](Object v) override { throw Error("int doesn't have a member <operator[](int)>"); }

        private:
            int value;
        };
        class Double : public Value
        {
        public:
            Double(double v) : value(v) {}
            std::string to_string() const override
            {
                return std::to_string(value);
            }
            type_code_index get_type() const override
            {
                return BasicType::DOUBLE;
            }
            Object operator=(Object v) override
            {
                value = std::static_pointer_cast<Double>(v)->value;
                return v;
            }
            Object operator+=(Object v) override
            {
                return std::make_shared<Double>(value +=
                                                std::static_pointer_cast<Double>(v)->value);
            }
            Object operator-=(Object v) override
            {
                return std::make_shared<Double>(value -=
                                                std::static_pointer_cast<Double>(v)->value);
            }
            Object operator*=(Object v) override
            {
                return std::make_shared<Double>(value *=
                                                std::static_pointer_cast<Double>(v)->value);
            }
            Object operator/=(Object v) override
            {
                return std::make_shared<Double>(value /=
                                                std::static_pointer_cast<Double>(v)->value);
            }
            Object operator+(Object v) override
            {
                return std::make_shared<Double>(value +
                                                std::static_pointer_cast<Double>(v)->value);
            }
            Object operator-(Object v) override
            {
                return std::make_shared<Double>(value -
                                                std::static_pointer_cast<Double>(v)->value);
            }
            Object operator*(Object v) override
            {
                return std::make_shared<Double>(value *
                                                std::static_pointer_cast<Double>(v)->value);
            }
            Object operator/(Object v) override
            {
                return std::make_shared<Double>(value /
                                                std::static_pointer_cast<Double>(v)->value);
            }
            Object operator<(Object v) override
            {
                return std::make_shared<Bool>(value < std::static_pointer_cast<Double>(v)->value);
            }
            Object operator>(Object v) override
            {
                return std::make_shared<Bool>(value > std::static_pointer_cast<Double>(v)->value);
            }
            Object operator<=(Object v) override
            {
                return std::make_shared<Bool>(value <= std::static_pointer_cast<Double>(v)->value);
            }
            Object operator>=(Object v) override
            {
                return std::make_shared<Bool>(value >= std::static_pointer_cast<Double>(v)->value);
            }
            Object operator==(Object v) override
            {
                return std::make_shared<Bool>(value == std::static_pointer_cast<Double>(v)->value);
            }
            Object operator!=(Object v) override
            {
                return std::make_shared<Bool>(value != std::static_pointer_cast<Double>(v)->value);
            }
            Object clone() const override
            {
                return std::make_shared<Double>(value);
            }
            Object get_negation() override
            {
                return std::make_shared<Double>(-value);
            }
            Object operator[](Object v) override { throw Error("double doesn't have a member <operator[](int)>"); }
            double get_value()
            {
                return value;
            }
            Object Convert(type_code_index type) override;

        private:
            double value;
        };
        class String : public Value
        {
        public:
            String(const std::string &v) : str(v) {}
            String(char ch) : str(std::string(1, ch)) {}
            Object operator+(Object v) override
            {
                return std::make_shared<String>(str +
                                                std::static_pointer_cast<String>(v)->str);
            }
            Object operator=(Object v) override
            {
                str = std::static_pointer_cast<String>(v)->str;
                return v;
            }
            type_code_index get_type() const override
            {
                return BasicType::STRING;
            }
            Object Convert(type_code_index type) override
            {
                return std::make_shared<String>(str);
            }
            Object operator+=(Object v) override
            {
                return std::make_shared<String>(str += std::static_pointer_cast<String>(v)->str);
            }
            Object operator>(Object v) override
            {
                return std::make_shared<Bool>(str > std::static_pointer_cast<String>(v)->str);
            }
            Object operator<(Object v) override
            {
                return std::make_shared<Bool>(str < std::static_pointer_cast<String>(v)->str);
            }
            Object operator>=(Object v) override
            {
                return std::make_shared<Bool>(str >= std::static_pointer_cast<String>(v)->str);
            }
            Object operator<=(Object v) override
            {
                return std::make_shared<Bool>(str <= std::static_pointer_cast<String>(v)->str);
            }
            Object operator!=(Object v) override
            {
                return std::make_shared<Bool>(str != std::static_pointer_cast<String>(v)->str);
            }
            Object operator==(Object v) override
            {
                return std::make_shared<Bool>(str == std::static_pointer_cast<String>(v)->str);
            }
            Object operator[](Object v) override;
            Object clone() const override
            {
                return std::make_shared<String>(str);
            }
            std::string to_string() const override
            {
                return str;
            }
            std::string str;
        };
        class Pointer : public Value
        {
        public:
            Pointer(size_t s) = delete;
            Pointer(Object _obj);
            Mem::Object operator=(Object v) override;
            Mem::Object operator==(Object v) override;
            Mem::Object operator!=(Object v) override;
            Mem::Object clone() const override;
            Mem::Object rm_ref() { return obj; }
            Object operator[](Object v) override;
            std::string to_string() const override
            {
                return "pointer " + obj->to_string();
            }
            ~Pointer();

        private:
            Object obj;
        };
        class Char : public Value
        {
        public:
            Char(char v) : value(new char(v)) {}
            Char(char *v) : value(v), del(false) {}
            std::string to_string() const override
            {
                return std::string(1, *value);
            }
            type_code_index get_type() const override
            {
                return BasicType::CHAR;
            }
            Object operator=(Object v) override;
            Object operator+=(Object v) override
            {
                return std::make_shared<Char>(*value += *std::static_pointer_cast<Char>(v)->value);
            }
            Object operator-=(Object v) override
            {
                return std::make_shared<Char>(*value -= *std::static_pointer_cast<Char>(v)->value);
            }
            Object operator*=(Object v) override
            {
                return std::make_shared<Char>(*value *= *std::static_pointer_cast<Char>(v)->value);
            }
            Object operator/=(Object v) override
            {
                return std::make_shared<Char>(*value /= *std::static_pointer_cast<Char>(v)->value);
            }
            Object operator+(Object v) override
            {
                return std::make_shared<Char>(*value + *std::static_pointer_cast<Char>(v)->value);
            }
            Object operator-(Object v) override
            {
                return std::make_shared<Char>(*value -
                                              *std::static_pointer_cast<Char>(v)->value);
            }
            Object operator*(Object v) override
            {
                return std::make_shared<Char>(*value *
                                              *std::static_pointer_cast<Char>(v)->value);
            }
            Object operator/(Object v) override
            {
                return std::make_shared<Char>(*value /
                                              *std::static_pointer_cast<Char>(v)->value);
            }
            Object operator>(Object v) override
            {
                return std::make_shared<Bool>(*value > *std::static_pointer_cast<Char>(v)->value);
            }
            Object operator>=(Object v) override
            {
                return std::make_shared<Bool>(*value >= *std::static_pointer_cast<Char>(v)->value);
            }
            Object operator<(Object v) override
            {
                return std::make_shared<Bool>(*value < *std::static_pointer_cast<Char>(v)->value);
            }
            Object operator<=(Object v) override
            {
                return std::make_shared<Bool>(*value <= *std::static_pointer_cast<Char>(v)->value);
            }
            Object operator==(Object v) override
            {
                return std::make_shared<Bool>(*value == *std::static_pointer_cast<Char>(v)->value);
            }
            Object operator!=(Object v) override
            {
                return std::make_shared<Bool>(*value != *std::static_pointer_cast<Char>(v)->value);
            }
            Object clone() const override
            {
                return std::make_shared<Char>(*value);
            }
            Object get_negation() override
            {
                return std::make_shared<Char>(-*value);
            }
            char get_value()
            {
                return *value;
            }
            Object Convert(type_code_index type) override {}
            Object operator[](Object v) override { throw Error("char doesn't have a member <operator[](int)>"); }
            virtual ~Char()
            {
                if (del)
                    delete value;
            }

        private:
            bool del = true;
            char *value;
        };
        class AnyObj : public Mem::Value
        {
        public:
            template <typename T>
            AnyObj(const T &t) : obj(t) {}
            template <typename T>
            AnyObj &operator=(const T &t)
            {
                obj = t;
                return *this;
            }
            template <typename T>
            AnyObj &operator==(T &&t) noexcept
            {
                obj = t;
            }
            template <typename T>
            T &cast()
            {
                return mer::any_cast<T &>(obj);
            }
            Mem::Object clone() const override {}

        private:
            mer::any obj;
        };
        class Array : public Value
        {
        public:
            Array(type_code_index _type, int _pos, int _length) : type(_type), pos(_pos), length(_length) {}
            Mem::Object operator[](Object index) override;
            Mem::Object clone() const override;
            type_code_index get_type() const override { return type; }

        private:
            // element type
            type_code_index type;
            int pos;
            int length;
        };
        class GArray : public Value
        {
        public:
            GArray(type_code_index _type, int _pos, int _length) : type(_type), pos(_pos), length(_length) {}
            Mem::Object operator[](Object index) override;
            Mem::Object clone() const override;
            type_code_index get_type() const override { return type; }

        private:
            // element type
            type_code_index type;
            int pos;
            int length;
        };
    }
}