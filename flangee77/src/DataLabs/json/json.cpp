#include "json.h"

#include <utility>



namespace dl7::json {



    const object_t& json::as_object() const
    {
        assert(is_object());
        return std::get<object_t>(_value);
    }

    object_t& json::as_object()
    {
        return const_cast<object_t&>(std::as_const(*this).as_object()); // NOLINT(cppcoreguidelines-pro-type-const-cast)
    }

    const array_t& json::as_array() const
    {
        assert(is_array());
        return std::get<array_t>(_value);
    }

    array_t& json::as_array()
    {
        return const_cast<array_t&>(std::as_const(*this).as_array()); // NOLINT(cppcoreguidelines-pro-type-const-cast)
    }

    const string_t& json::as_string() const
    {
        assert(is_string());
        return std::get<string_t>(_value);
    }

    string_t& json::as_string()
    {
        return const_cast<string_t&>(std::as_const(*this).as_string()); // NOLINT(cppcoreguidelines-pro-type-const-cast)
    }

    number_real_t json::as_number_real() const
    {
        assert(is_number_real());
        return std::get<number_real_t>(_value);
    }

    number_integer_t json::as_number_integer() const
    {
        assert(is_number_integer());
        return std::get<number_integer_t>(_value);
    }

    number_unsigned_t json::as_number_unsigned() const
    {
        assert(is_number_unsigned());
        return std::get<number_unsigned_t>(_value);
    }

    boolean_t json::as_boolean() const
    {
        assert(is_boolean());
        return std::get<boolean_t>(_value);
    }

    void json::set_null()
    {
        _value = nullptr;
    }

    void json::set_object(object_t object)
    {
        _value = std::move(object);
    }

    void json::set_array(array_t array)
    {
        _value = std::move(array);
    }

    void json::set_string(string_t string)
    {
        _value = std::move(string);
    }

    void json::set_number_real(number_real_t number)
    {
        _value = number;
    }

    void json::set_number_integer(number_integer_t number)
    {
        _value = number;
    }

    void json::set_number_unsigned(number_unsigned_t number)
    {
        _value = number;
    }

    void json::set_boolean(boolean_t boolean)
    {
        _value = boolean;
    }



} // namespace dl7::json
