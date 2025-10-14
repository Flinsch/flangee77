#include "Json.h"

#include "./JsonWriter.h"

#include <utility>



namespace dl7::json {



    Json::Json()
        : _value(nullptr)
    {
    }

    Json::Json(null_t)
        : _value(nullptr)
    {
    }

    Json::Json(object_t object)
        : _value(std::make_unique<object_t>(std::move(object)))
    {
    }

    Json::Json(array_t array)
        : _value(std::make_unique<array_t>(std::move(array)))
    {
    }

    Json::Json(string_t string)
        : _value(std::move(string))
    {
    }

    Json::Json(std::basic_string_view<string_t::value_type> string)
        : _value(string_t{string})
    {
    }

    Json::Json(const string_t::value_type* string)
        : _value(string_t{string})
    {
    }

    Json::Json(float number) { _set_float(static_cast<float_t>(number)); }
    Json::Json(double number) { _set_float(static_cast<float_t>(number)); }
    Json::Json(long double number) { _set_float(static_cast<float_t>(number)); }

    Json::Json(signed number) { _set_integer(static_cast<integer_t>(number)); }
    Json::Json(signed long number) { _set_integer(static_cast<integer_t>(number)); }
    Json::Json(signed long long number) { _set_integer(static_cast<integer_t>(number)); }

    Json::Json(unsigned number) { _set_integer(static_cast<integer_t>(number)); }
    Json::Json(unsigned long number) { _set_integer(static_cast<integer_t>(number)); }
    Json::Json(unsigned long long number) { _set_integer(static_cast<integer_t>(number)); }

    Json::Json(boolean_t boolean)
        : _value(boolean)
    {
    }

    Json::Json(Type type)
    {
        reset_type(type);
    }

    Json::Json(const Json& other)
    {
        *this = other;
    }

    Json& Json::operator=(const Json& other)
    {
        switch (other.get_type())
        {
        case Type::Object:
            set_object(other.as_object());
            break;
        case Type::Array:
            set_array(other.as_array());
            break;
        case Type::String:
            set_string(other.as_string());
            break;
        case Type::Float:
            set_float(other.as_float());
            break;
        case Type::Integer:
            set_integer(other.as_integer());
            break;
        case Type::Boolean:
            set_boolean(other.as_boolean());
            break;
        default:
            assert(is_null());
        }

        return *this;
    }

    Json::Json(Json&& other) noexcept
        : _value(std::move(other._value))
    {
    }

    Json& Json::operator=(Json&& other) noexcept
    {
        _value = std::move(other._value);
        return *this;
    }

    void Json::swap(Json& other) noexcept
    {
        _value.swap(other._value);
    }



    /**
     * Returns true if this JSON value represents null, an empty object, an empty
     * array, or an empty string; returns false otherwise (i.e., numbers, including
     * 0, and booleans, including false, are considered non-empty).
     */
    bool Json::is_empty() const noexcept
    {
        if (is_null()) return true;

        if (is_object()) { const object_ptr_t* object_ptr = std::get_if<object_ptr_t>(&_value); return object_ptr ? (*object_ptr)->empty() : true; }
        if (is_array()) { const array_ptr_t* array_ptr = std::get_if<array_ptr_t>(&_value); return array_ptr ? (*array_ptr)->empty() : true; }
        if (is_string()) { const string_t* string = std::get_if<string_t>(&_value); return string ? string->empty() : true; }

        return false;
    }



    const object_t& Json::as_object() const
    {
        assert(is_object());
        return *std::get<object_ptr_t>(_value);
    }

    object_t& Json::as_object()
    {
        return const_cast<object_t&>(std::as_const(*this).as_object()); // NOLINT(cppcoreguidelines-pro-type-const-cast)
    }

    const array_t& Json::as_array() const
    {
        assert(is_array());
        return *std::get<array_ptr_t>(_value);
    }

    array_t& Json::as_array()
    {
        return const_cast<array_t&>(std::as_const(*this).as_array()); // NOLINT(cppcoreguidelines-pro-type-const-cast)
    }

    const string_t& Json::as_string() const
    {
        assert(is_string());
        return std::get<string_t>(_value);
    }

    string_t& Json::as_string()
    {
        return const_cast<string_t&>(std::as_const(*this).as_string()); // NOLINT(cppcoreguidelines-pro-type-const-cast)
    }

    float_t Json::as_float() const
    {
        return _as_float();
    }

    integer_t Json::as_integer() const
    {
        return _as_integer();
    }

    boolean_t Json::as_boolean() const
    {
        assert(is_boolean());
        return std::get<boolean_t>(_value);
    }

    void Json::set_null()
    {
        _value = nullptr;
    }

    void Json::set_object(object_t object)
    {
        _value = std::make_unique<object_t>(std::move(object));
    }

    void Json::set_array(array_t array)
    {
        _value = std::make_unique<array_t>(std::move(array));
    }

    void Json::set_boolean(boolean_t boolean)
    {
        _value = boolean;
    }

    void Json::reset_type(Type type)
    {
        switch (type)
        {
        case Type::Null:
            set_null();
            break;
        case Type::Object:
            set_object({});
            break;
        case Type::Array:
            set_array({});
            break;
        case Type::String:
            set_string(string_t{});
            break;
        case Type::Float:
            set_float(float_t{});
            break;
        case Type::Integer:
            set_integer(integer_t{});
            break;
        case Type::Boolean:
            set_boolean(boolean_t{});
            break;
        default:
            assert(false);
            set_null();
        }
    }



    /**
     * Serializes this JSON value into a compact single-line string representation.
     */
    string_t Json::to_string() const
    {
        return JsonWriter::to_string(*this, JsonWriter::DEFAULT_COMPACT_FORMAT);
    }



    const Json& Json::at(size_t index) const
    {
        return as_array().at(index);
    }

    Json& Json::at(size_t index)
    {
        return as_array().at(index);
    }

    const Json& Json::at(cl7::u8string_view key) const
    {
        return as_object().at(key);
    }

    Json& Json::at(cl7::u8string_view key)
    {
        return as_object().at(key);
    }



    Json& Json::operator[](size_t index)
    {
        if (is_null())
            set_array({});
        return as_array()[index];
    }

    Json& Json::operator[](const cl7::u8string& key)
    {
        if (is_null())
            set_object({});
        return as_object()[key];
    }

    Json& Json::operator[](cl7::u8string&& key)
    {
        if (is_null())
            set_object({});
        return as_object()[std::move(key)];
    }



    bool Json::operator==(const Json& other) const
    {
        if (get_type() != other.get_type())
            return false;

        switch (get_type())
        {
        case Type::Object:
            return as_object() == other.as_object();
        case Type::Array:
            return as_array() == other.as_array();
        default:
            return _value == other._value;
        }
    }



    float_t Json::_as_float() const
    {
        assert(is_float());
        return std::get<float_t>(_value);
    }

    integer_t Json::_as_integer() const
    {
        assert(is_integer());
        return std::get<integer_t>(_value);
    }

    void Json::_set_string(string_t&& string)
    {
        _value = std::move(string);
    }

    void Json::_set_float(float_t number)
    {
        _value = number;
    }

    void Json::_set_integer(integer_t number)
    {
        _value = number;
    }



} // namespace dl7::json
