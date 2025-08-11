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
        Decimal,
        Integer,
        Unsigned,
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
    bool is_decimal() const noexcept { return std::holds_alternative<decimal_t>(_value); }
    bool is_integer() const noexcept { return std::holds_alternative<integer_t>(_value); }
    bool is_unsigned() const noexcept { return std::holds_alternative<unsigned_t>(_value); }
    bool is_number() const noexcept { return is_decimal() || is_integer() || is_unsigned(); }
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

    decimal_t as_decimal() const;
    integer_t as_integer() const;
    unsigned_t as_unsigned() const;

    template <typename Tdecimal = float>
        requires(std::is_floating_point_v<Tdecimal>)
    Tdecimal as_decimal() const { return static_cast<Tdecimal>(_as_decimal()); }

    template <typename Tinteger = signed>
        requires(std::is_integral_v<Tinteger> && std::is_signed_v<Tinteger>)
    Tinteger as_integer() const { return static_cast<Tinteger>(_as_integer()); }

    template <typename Tunsigned = unsigned>
        requires(std::is_integral_v<Tunsigned> && std::is_unsigned_v<Tunsigned>)
    Tunsigned as_unsigned() const { return static_cast<Tunsigned>(_as_unsigned()); }

    boolean_t as_boolean() const;



    void set_undefined();
    void set_string(string_t string);
    void set_string(std::basic_string_view<string_t::value_type> string);
    void set_string(const string_t::value_type* string);

    template <typename Tdecimal>
        requires(std::is_nothrow_convertible_v<Tdecimal, decimal_t>)
    void set_decimal(Tdecimal number) { _set_decimal(static_cast<decimal_t>(number)); }

    template <typename Tinteger>
        requires(std::is_nothrow_convertible_v<Tinteger, integer_t>)
    void set_integer(Tinteger number) { _set_integer(static_cast<integer_t>(number)); }

    template <typename Tunsigned>
        requires(std::is_nothrow_convertible_v<Tunsigned, unsigned_t>)
    void set_unsigned(Tunsigned number) { _set_unsigned(static_cast<unsigned_t>(number)); }

    void set_boolean(boolean_t boolean);

    void reset_type(Type type);



    /**
     * Serializes this INI value into a string representation.
     */
    string_t to_string() const;



    bool operator==(const Value& other) const = default;
    bool operator!=(const Value& other) const { return !(*this == other); }



private:
    decimal_t _as_decimal() const;
    integer_t _as_integer() const;
    unsigned_t _as_unsigned() const;

    void _set_decimal(decimal_t number);
    void _set_integer(integer_t number);
    void _set_unsigned(unsigned_t number);

    std::variant<undefined_t, string_t, decimal_t, integer_t, unsigned_t, boolean_t> _value;

}; // class Value



} // namespace dl7::ini

#endif // DL7_INI_VALUE_H
