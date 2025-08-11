#include "Value.h"

//#include "./IniWriter.h"

#include <utility>



namespace dl7::ini {



    Value::Value()
        : _value(nullptr)
    {
    }

    Value::Value(string_t string)
        : _value(std::move(string))
    {
    }

    Value::Value(std::basic_string_view<string_t::value_type> string)
        : _value(string_t{string})
    {
    }

    Value::Value(const string_t::value_type* string)
        : _value(string_t{string})
    {
    }

    Value::Value(float number) { _set_decimal(number); }
    Value::Value(double number) { _set_decimal(number); }
    Value::Value(long double number) { _set_decimal(static_cast<decimal_t>(number)); }

    Value::Value(signed number) { _set_integer(number); }
    Value::Value(signed long number) { _set_integer(number); }
    Value::Value(signed long long number) { _set_integer(number); }

    Value::Value(unsigned number) { _set_unsigned(number); }
    Value::Value(unsigned long number) { _set_unsigned(number); }
    Value::Value(unsigned long long number) { _set_unsigned(number); }

    Value::Value(boolean_t boolean)
        : _value(boolean)
    {
    }

    Value::Value(Type type)
    {
        reset_type(type);
    }

    Value::Value(const Value& other)
    {
        *this = other;
    }

    Value& Value::operator=(const Value& other)
    {
        switch (other.get_type())
        {
        case Type::String:
            set_string(other.as_string());
            break;
        case Type::Decimal:
            set_decimal(other.as_decimal());
            break;
        case Type::Integer:
            set_integer(other.as_integer());
            break;
        case Type::Unsigned:
            set_unsigned(other.as_unsigned());
            break;
        case Type::Boolean:
            set_boolean(other.as_boolean());
            break;
        default:
            assert(is_undefined());
        }

        return *this;
    }

    Value::Value(Value&& other) noexcept
        : _value(std::move(other._value))
    {
    }

    Value& Value::operator=(Value&& other) noexcept
    {
        _value = std::move(other._value);
        return *this;
    }

    void Value::swap(Value& other) noexcept
    {
        _value.swap(other._value);
    }



    /**
     * Returns true if this INI value is undefined or represents an empty string;
     * returns false otherwise (i.e., numbers, including 0, and booleans, including
     * false, are considered non-empty).
     */
    bool Value::is_empty() const noexcept
    {
        if (is_undefined()) return true;

        if (is_string()) { const string_t* string = std::get_if<string_t>(&_value); return string ? string->empty() : true; }

        return false;
    }



    const string_t& Value::as_string() const
    {
        assert(is_string());
        return std::get<string_t>(_value);
    }

    string_t& Value::as_string()
    {
        return const_cast<string_t&>(std::as_const(*this).as_string()); // NOLINT(cppcoreguidelines-pro-type-const-cast)
    }

    decimal_t Value::as_decimal() const
    {
        return _as_decimal();
    }

    integer_t Value::as_integer() const
    {
        return _as_integer();
    }

    unsigned_t Value::as_unsigned() const
    {
        return _as_unsigned();
    }

    boolean_t Value::as_boolean() const
    {
        assert(is_boolean());
        return std::get<boolean_t>(_value);
    }

    void Value::set_undefined()
    {
        _value = nullptr;
    }

    void Value::set_string(string_t string)
    {
        _value = std::move(string);
    }

    void Value::set_string(std::basic_string_view<string_t::value_type> string)
    {
        _value = string_t{string};
    }

    void Value::set_string(const string_t::value_type* string)
    {
        _value = string_t{string};
    }

    void Value::set_boolean(boolean_t boolean)
    {
        _value = boolean;
    }

    void Value::reset_type(Type type)
    {
        switch (type)
        {
        case Type::Undefined:
            set_undefined();
            break;
        case Type::String:
            set_string({});
            break;
        case Type::Decimal:
            set_decimal(decimal_t{});
            break;
        case Type::Integer:
            set_integer(integer_t{});
            break;
        case Type::Unsigned:
            set_unsigned(unsigned_t{});
            break;
        case Type::Boolean:
            set_boolean(boolean_t{});
            break;
        default:
            assert(false);
            set_undefined();
        }
    }



    /**
     * Serializes this INI value into a string representation.
     */
    string_t Value::to_string() const
    {
        if (is_undefined()) return {};
        if (is_string()) return as_string();
        if (is_integer()) return cl7::to_string(as_integer());
        if (is_unsigned()) return cl7::to_string(as_unsigned());
        if (is_decimal()) return cl7::to_string(as_decimal());
        if (is_true()) return u8"true";
        if (is_false()) return u8"false";

        //return IniWriter::to_string(*this);
        assert(false);
        return {};
    }



    decimal_t Value::_as_decimal() const
    {
        assert(is_decimal());
        return std::get<decimal_t>(_value);
    }

    integer_t Value::_as_integer() const
    {
        assert(is_integer());
        return std::get<integer_t>(_value);
    }

    unsigned_t Value::_as_unsigned() const
    {
        assert(is_unsigned());
        return std::get<unsigned_t>(_value);
    }

    void Value::_set_decimal(decimal_t number)
    {
        _value = number;
    }

    void Value::_set_integer(integer_t number)
    {
        _value = number;
    }

    void Value::_set_unsigned(unsigned_t number)
    {
        _value = number;
    }



} // namespace dl7::ini
