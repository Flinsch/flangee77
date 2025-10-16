#ifndef DL7_INI_VALUE_H
#define DL7_INI_VALUE_H

#include "./types.h"

#include <variant>



namespace dl7::ini {



class Value
{

public:
    enum struct Type
    {
        Undefined,
        String,
        Float,
        Integer,
        Boolean,
    };



    Value();
    explicit Value(string_t string);
    explicit Value(std::basic_string_view<string_t::value_type> string);
    explicit Value(const string_t::value_type* string);

    explicit Value(float number);
    explicit Value(double number);
    explicit Value(long double number);

    explicit Value(signed number);
    explicit Value(signed long number);
    explicit Value(signed long long number);

    explicit Value(unsigned number);
    explicit Value(unsigned long number);
    explicit Value(unsigned long long number);

    explicit Value(boolean_t boolean);

    explicit Value(Type type);

    Value(const Value& other);
    Value& operator=(const Value& other);
    Value(Value&& other) noexcept;
    Value& operator=(Value&& other) noexcept;

    ~Value() noexcept = default;

    void swap(Value& other) noexcept;



    Type get_type() const noexcept { return static_cast<Type>(_value.index()); }

    bool is_undefined() const noexcept { return std::holds_alternative<undefined_t>(_value); }
    bool is_string() const noexcept { return std::holds_alternative<string_t>(_value); }
    bool is_float() const noexcept { return std::holds_alternative<float_t>(_value); }
    bool is_integer() const noexcept { return std::holds_alternative<integer_t>(_value); }
    bool is_number() const noexcept { return is_float() || is_integer(); }
    bool is_boolean() const noexcept { return std::holds_alternative<boolean_t>(_value); }
    bool is_true() const noexcept { const boolean_t* b = std::get_if<boolean_t>(&_value); return b ? *b : false; }
    bool is_false() const noexcept { const boolean_t* b = std::get_if<boolean_t>(&_value); return b ? !*b : false; }

    /**
     * Returns true if this INI value is undefined or represents an empty string;
     * returns false otherwise (i.e., numbers, including 0, and booleans, including
     * false, are considered non-empty).
     */
    bool is_empty() const noexcept;



    const string_t& as_string() const;
    string_t& as_string();

    float_t as_float() const;
    integer_t as_integer() const;

    template <std::floating_point Tfloat = float>
    Tfloat as_float() const { return static_cast<Tfloat>(_as_float()); }

    template <std::integral Tinteger = signed>
    Tinteger as_integer() const { return static_cast<Tinteger>(_as_integer()); }

    template <typename Tnumber>
        requires(std::is_arithmetic_v<Tnumber>)
    Tnumber as_number() const
    {
        switch (get_type())
        {
        case Type::Integer:
            return static_cast<Tnumber>(_as_integer());
        default:
            return static_cast<Tnumber>(_as_float());
        }
    }

    boolean_t as_boolean() const;



    void set_undefined();

    template <cl7::string_constructible<string_t> Tstring>
    void set_string(Tstring&& string) { _set_string(string_t(std::forward<Tstring>(string))); }

    template <std::convertible_to<float_t> Tnumber>
    void set_float(Tnumber number) { _set_float(static_cast<float_t>(number)); }

    template <std::convertible_to<integer_t> Tnumber>
    void set_integer(Tnumber number) { _set_integer(static_cast<integer_t>(number)); }

    template <std::floating_point Tnumber>
    void set_number(Tnumber number) { _set_float(static_cast<float_t>(number)); }

    template <std::integral Tnumber>
    void set_number(Tnumber number) { _set_integer(static_cast<integer_t>(number)); }

    void set_boolean(boolean_t boolean);

    void reset_type(Type type);



    /**
     * Serializes this INI value into a string representation.
     */
    string_t to_string() const;



    bool operator==(const Value& other) const = default;
    bool operator!=(const Value& other) const { return !(*this == other); }



private:
    float_t _as_float() const;
    integer_t _as_integer() const;

    void _set_string(string_t&& string);

    void _set_float(float_t number);
    void _set_integer(integer_t number);

    std::variant<undefined_t, string_t, float_t, integer_t, boolean_t> _value;

}; // class Value



} // namespace dl7::ini

#endif // DL7_INI_VALUE_H
