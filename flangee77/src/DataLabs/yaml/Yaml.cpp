#include "Yaml.h"

#include "./YamlWriter.h"

#include <utility>



namespace dl7::yaml {



    Yaml::Yaml()
        : _value(nullptr)
    {
    }

    Yaml::Yaml(null_t)
        : _value(nullptr)
    {
    }

    Yaml::Yaml(mapping_t mapping)
        : _value(std::make_unique<mapping_t>(std::move(mapping)))
    {
    }

    Yaml::Yaml(sequence_t sequence)
        : _value(std::make_unique<sequence_t>(std::move(sequence)))
    {
    }

    Yaml::Yaml(string_t string)
        : _value(std::move(string))
    {
    }

    Yaml::Yaml(std::basic_string_view<string_t::value_type> string)
        : _value(string_t{string})
    {
    }

    Yaml::Yaml(const string_t::value_type* string)
        : _value(string_t{string})
    {
    }

    Yaml::Yaml(float number) { _set_float(static_cast<float_t>(number)); }
    Yaml::Yaml(double number) { _set_float(static_cast<float_t>(number)); }
    Yaml::Yaml(long double number) { _set_float(static_cast<float_t>(number)); }

    Yaml::Yaml(signed number) { _set_integer(static_cast<integer_t>(number)); }
    Yaml::Yaml(signed long number) { _set_integer(static_cast<integer_t>(number)); }
    Yaml::Yaml(signed long long number) { _set_integer(static_cast<integer_t>(number)); }

    Yaml::Yaml(unsigned number) { _set_integer(static_cast<integer_t>(number)); }
    Yaml::Yaml(unsigned long number) { _set_integer(static_cast<integer_t>(number)); }
    Yaml::Yaml(unsigned long long number) { _set_integer(static_cast<integer_t>(number)); }

    Yaml::Yaml(boolean_t boolean)
        : _value(boolean)
    {
    }

    Yaml::Yaml(Type type)
    {
        reset_type(type);
    }

    Yaml::Yaml(const Yaml& other)
    {
        *this = other;
    }

    Yaml& Yaml::operator=(const Yaml& other)
    {
        switch (other.get_type())
        {
        case Type::Mapping:
            set_mapping(other.as_mapping());
            break;
        case Type::Sequence:
            set_sequence(other.as_sequence());
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
            set_null();
        }

        return *this;
    }

    Yaml::Yaml(Yaml&& other) noexcept
        : _value(std::move(other._value))
    {
    }

    Yaml& Yaml::operator=(Yaml&& other) noexcept
    {
        _value = std::move(other._value);
        return *this;
    }

    void Yaml::swap(Yaml& other) noexcept
    {
        _value.swap(other._value);
    }



    /**
     * Returns true if this YAML node represents null, an empty mapping, an empty
     * sequence, or an empty string; returns false otherwise (i.e., numbers,
     * including 0, and booleans, including false, are considered non-empty).
     */
    bool Yaml::is_empty() const noexcept
    {
        if (is_null()) return true;

        if (is_mapping()) { const mapping_ptr_t* mapping_ptr = std::get_if<mapping_ptr_t>(&_value); return mapping_ptr ? (*mapping_ptr)->empty() : true; }
        if (is_sequence()) { const sequence_ptr_t* sequence_ptr = std::get_if<sequence_ptr_t>(&_value); return sequence_ptr ? (*sequence_ptr)->empty() : true; }
        if (is_string()) { const string_t* string = std::get_if<string_t>(&_value); return string ? string->empty() : true; }

        return false;
    }



    const mapping_t& Yaml::as_mapping() const
    {
        assert(is_mapping());
        return *std::get<mapping_ptr_t>(_value);
    }

    mapping_t& Yaml::as_mapping()
    {
        return const_cast<mapping_t&>(std::as_const(*this).as_mapping()); // NOLINT(cppcoreguidelines-pro-type-const-cast)
    }

    const sequence_t& Yaml::as_sequence() const
    {
        assert(is_sequence());
        return *std::get<sequence_ptr_t>(_value);
    }

    sequence_t& Yaml::as_sequence()
    {
        return const_cast<sequence_t&>(std::as_const(*this).as_sequence()); // NOLINT(cppcoreguidelines-pro-type-const-cast)
    }

    const string_t& Yaml::as_string() const
    {
        assert(is_string());
        return std::get<string_t>(_value);
    }

    string_t& Yaml::as_string()
    {
        return const_cast<string_t&>(std::as_const(*this).as_string()); // NOLINT(cppcoreguidelines-pro-type-const-cast)
    }

    float_t Yaml::as_float() const
    {
        return _as_float();
    }

    integer_t Yaml::as_integer() const
    {
        return _as_integer();
    }

    boolean_t Yaml::as_boolean() const
    {
        assert(is_boolean());
        return std::get<boolean_t>(_value);
    }

    void Yaml::set_null()
    {
        _value = nullptr;
    }

    void Yaml::set_mapping(mapping_t mapping)
    {
        _value = std::make_unique<mapping_t>(std::move(mapping));
    }

    void Yaml::set_sequence(sequence_t sequence)
    {
        _value = std::make_unique<sequence_t>(std::move(sequence));
    }

    void Yaml::set_boolean(boolean_t boolean)
    {
        _value = boolean;
    }

    void Yaml::reset_type(Type type)
    {
        switch (type)
        {
        case Type::Null:
            set_null();
            break;
        case Type::Mapping:
            set_mapping({});
            break;
        case Type::Sequence:
            set_sequence({});
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
     * Serializes this YAML node into a single-line flow-style string representation.
     */
    string_t Yaml::to_string() const
    {
        return YamlWriter::to_string(*this, YamlWriter::DEFAULT_FLOW_FORMAT);
    }



    const Yaml& Yaml::at(size_t index) const
    {
        return as_sequence().at(index);
    }

    Yaml& Yaml::at(size_t index)
    {
        return as_sequence().at(index);
    }

    const Yaml& Yaml::at(cl7::u8string_view key) const
    {
        return as_mapping().at(key);
    }

    Yaml& Yaml::at(cl7::u8string_view key)
    {
        return as_mapping().at(key);
    }



    Yaml& Yaml::operator[](size_t index)
    {
        if (is_null())
            set_sequence({});
        return as_sequence()[index];
    }

    Yaml& Yaml::operator[](const cl7::u8string& key)
    {
        if (is_null())
            set_mapping({});
        return as_mapping()[key];
    }

    Yaml& Yaml::operator[](cl7::u8string&& key)
    {
        if (is_null())
            set_mapping({});
        return as_mapping()[std::move(key)];
    }



    bool Yaml::operator==(const Yaml& other) const
    {
        if (get_type() != other.get_type())
            return false;

        switch (get_type())
        {
        case Type::Mapping:
            return as_mapping() == other.as_mapping();
        case Type::Sequence:
            return as_sequence() == other.as_sequence();
        default:
            return _value == other._value;
        }
    }



    float_t Yaml::_as_float() const
    {
        assert(is_float());
        return std::get<float_t>(_value);
    }

    integer_t Yaml::_as_integer() const
    {
        assert(is_integer());
        return std::get<integer_t>(_value);
    }

    void Yaml::_set_string(string_t&& string)
    {
        _value = std::move(string);
    }

    void Yaml::_set_float(float_t number)
    {
        _value = number;
    }

    void Yaml::_set_integer(integer_t number)
    {
        _value = number;
    }



} // namespace dl7::yaml
