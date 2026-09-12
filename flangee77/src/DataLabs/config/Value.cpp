#include "Value.h"

#include <utility>



namespace dl7::config {



    Value::Value()
        : _value(nullptr)
    {
    }

    Value::Value(null_t)
        : _value(nullptr)
    {
    }

    Value::Value(mapping_t mapping)
        : _value(std::make_unique<mapping_t>(std::move(mapping)))
    {
    }

    Value::Value(sequence_t sequence)
        : _value(std::make_unique<sequence_t>(std::move(sequence)))
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

    Value::Value(float number) { _set_float(static_cast<float_t>(number)); }
    Value::Value(double number) { _set_float(static_cast<float_t>(number)); }
    Value::Value(long double number) { _set_float(static_cast<float_t>(number)); }

    Value::Value(signed number) { _set_integer(static_cast<integer_t>(number)); }
    Value::Value(signed long number) { _set_integer(static_cast<integer_t>(number)); }
    Value::Value(signed long long number) { _set_integer(static_cast<integer_t>(number)); }

    Value::Value(unsigned number) { _set_integer(static_cast<integer_t>(number)); }
    Value::Value(unsigned long number) { _set_integer(static_cast<integer_t>(number)); }
    Value::Value(unsigned long long number) { _set_integer(static_cast<integer_t>(number)); }

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
     * Returns true if this value represents null, an empty mapping, an empty
     * sequence, or an empty string; returns false otherwise (i.e., numbers,
     * including 0, and booleans, including false, are considered non-empty).
     */
    bool Value::is_empty() const noexcept
    {
        if (is_null()) return true;

        if (is_mapping()) { const mapping_ptr_t* mapping_ptr = std::get_if<mapping_ptr_t>(&_value); return mapping_ptr ? (*mapping_ptr)->empty() : true; }
        if (is_sequence()) { const sequence_ptr_t* sequence_ptr = std::get_if<sequence_ptr_t>(&_value); return sequence_ptr ? (*sequence_ptr)->empty() : true; }
        if (is_string()) { const string_t* string = std::get_if<string_t>(&_value); return string ? string->empty() : true; }

        return false;
    }



    const mapping_t& Value::as_mapping() const
    {
        assert(is_mapping());
        return *std::get<mapping_ptr_t>(_value);
    }

    mapping_t& Value::as_mapping()
    {
        return const_cast<mapping_t&>(std::as_const(*this).as_mapping()); // NOLINT(cppcoreguidelines-pro-type-const-cast)
    }

    const sequence_t& Value::as_sequence() const
    {
        assert(is_sequence());
        return *std::get<sequence_ptr_t>(_value);
    }

    sequence_t& Value::as_sequence()
    {
        return const_cast<sequence_t&>(std::as_const(*this).as_sequence()); // NOLINT(cppcoreguidelines-pro-type-const-cast)
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

    float_t Value::as_float() const
    {
        return _as_float();
    }

    integer_t Value::as_integer() const
    {
        return _as_integer();
    }

    boolean_t Value::as_boolean() const
    {
        assert(is_boolean());
        return std::get<boolean_t>(_value);
    }

    void Value::set_null()
    {
        _value = nullptr;
    }

    void Value::set_mapping(mapping_t mapping)
    {
        _value = std::make_unique<mapping_t>(std::move(mapping));
    }

    void Value::set_sequence(sequence_t sequence)
    {
        _value = std::make_unique<sequence_t>(std::move(sequence));
    }

    void Value::set_boolean(boolean_t boolean)
    {
        _value = boolean;
    }

    void Value::reset_type(Type type)
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
     * Overlays the given value onto this one, which is how a configuration read
     * from a file is laid over the defaults it may leave out. Two mappings merge
     * key by key, recursively. Anything else the overlay has it simply wins with,
     * a sequence being replaced rather than appended to and an explicit null
     * counting as a value like any other.
     */
    void Value::merge(const Value& overlay)
    {
        if (!is_mapping() || !overlay.is_mapping())
        {
            *this = overlay;
            return;
        }

        mapping_t& mapping = as_mapping();

        for (const auto& p : overlay.as_mapping())
        {
            const auto it = mapping.find(p.first);

            if (it == mapping.end())
                mapping[p.first] = p.second;
            else
                it->second.merge(p.second);
        }
    }



    const Value& Value::at(size_t index) const
    {
        return as_sequence().at(index);
    }

    Value& Value::at(size_t index)
    {
        return as_sequence().at(index);
    }

    const Value& Value::at(cl7::u8string_view key) const
    {
        return as_mapping().at(key);
    }

    Value& Value::at(cl7::u8string_view key)
    {
        return as_mapping().at(key);
    }



    Value& Value::operator[](size_t index)
    {
        if (is_null())
            set_sequence({});
        return as_sequence()[index];
    }

    Value& Value::operator[](const cl7::u8string& key)
    {
        if (is_null())
            set_mapping({});
        return as_mapping()[key];
    }

    Value& Value::operator[](cl7::u8string&& key)
    {
        if (is_null())
            set_mapping({});
        return as_mapping()[std::move(key)];
    }



    bool Value::operator==(const Value& other) const
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



    float_t Value::_as_float() const
    {
        assert(is_float());
        return std::get<float_t>(_value);
    }

    integer_t Value::_as_integer() const
    {
        assert(is_integer());
        return std::get<integer_t>(_value);
    }

    void Value::_set_string(string_t&& string)
    {
        _value = std::move(string);
    }

    void Value::_set_float(float_t number)
    {
        _value = number;
    }

    void Value::_set_integer(integer_t number)
    {
        _value = number;
    }



} // namespace dl7::config
