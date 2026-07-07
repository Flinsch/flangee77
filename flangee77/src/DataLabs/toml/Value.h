#ifndef DL7_TOML_VALUE_H
#define DL7_TOML_VALUE_H

#include "./types.h"

#include <variant>



namespace dl7::toml {



class Value
{

public:
    enum struct Type
    {
        None,
        String,
        Integer,
        Float,
        Boolean,
        OffsetDateTime,
        LocalDateTime,
        LocalDate,
        LocalTime,
        Array,
        Table,
    };



    Value();
    explicit Value(none_t);
    explicit Value(array_t array);
    explicit Value(table_t table);
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

    explicit Value(offset_date_time_t offset_date_time);
    explicit Value(local_date_time_t local_date_time);
    explicit Value(local_date_t local_date);
    explicit Value(local_time_t local_time);

    explicit Value(Type type);

    Value(const Value& other);
    Value& operator=(const Value& other);
    Value(Value&& other) noexcept;
    Value& operator=(Value&& other) noexcept;

    ~Value() noexcept = default;

    void swap(Value& other) noexcept;



    Type get_type() const noexcept { return static_cast<Type>(_value.index()); }

    bool is_none() const noexcept { return std::holds_alternative<none_t>(_value); }
    bool is_array() const noexcept { return std::holds_alternative<array_ptr_t>(_value); }
    bool is_table() const noexcept { return std::holds_alternative<table_ptr_t>(_value); }
    bool is_string() const noexcept { return std::holds_alternative<string_t>(_value); }
    bool is_float() const noexcept { return std::holds_alternative<float_t>(_value); }
    bool is_integer() const noexcept { return std::holds_alternative<integer_t>(_value); }
    bool is_number() const noexcept { return is_float() || is_integer(); }
    bool is_boolean() const noexcept { return std::holds_alternative<boolean_t>(_value); }
    bool is_true() const noexcept { const boolean_t* b = std::get_if<boolean_t>(&_value); return b ? *b : false; }
    bool is_false() const noexcept { const boolean_t* b = std::get_if<boolean_t>(&_value); return b ? !*b : false; }
    bool is_offset_date_time() const noexcept { return std::holds_alternative<offset_date_time_t>(_value); }
    bool is_local_date_time() const noexcept { return std::holds_alternative<local_date_time_t>(_value); }
    bool is_local_date() const noexcept { return std::holds_alternative<local_date_t>(_value); }
    bool is_local_time() const noexcept { return std::holds_alternative<local_time_t>(_value); }
    bool is_date_time() const noexcept { return is_offset_date_time() || is_local_date_time() || is_local_date() || is_local_time(); }
    bool is_primitive() const noexcept { return is_none() || is_string() || is_number() || is_boolean() || is_date_time(); }
    bool is_structured() const noexcept { return is_array() || is_table(); }

    /**
     * Returns true if this TOML value represents "none", an empty array, an empty
     * table, or an empty string; returns false otherwise (i.e., numbers, including
     * 0, and booleans, including false, are considered non-empty).
     */
    bool is_empty() const noexcept;



    const array_t& as_array() const;
    array_t& as_array();

    const table_t& as_table() const;
    table_t& as_table();

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

    offset_date_time_t as_offset_date_time() const;
    local_date_time_t as_local_date_time() const;
    local_date_t as_local_date() const;
    local_time_t as_local_time() const;



    void set_none();
    void set_array(array_t array);
    void set_table(table_t table);

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

    void set_offset_date_time(offset_date_time_t offset_date_time);
    void set_local_date_time(local_date_time_t local_date_time);
    void set_local_date(local_date_t local_date);
    void set_local_time(local_time_t local_time);

    void reset_type(Type type);



    /**
     * Serializes this TOML value into an inline string representation.
     */
    string_t to_string() const;



    const Value& at(size_t index) const;
    Value& at(size_t index);

    const Value& at(cl7::u8string_view key) const;
    Value& at(cl7::u8string_view key);



    Value& operator[](size_t index);

    Value& operator[](const cl7::u8string& key);
    Value& operator[](cl7::u8string&& key);



    bool operator==(const Value& other) const;
    bool operator!=(const Value& other) const { return !(*this == other); }



private:
    using array_ptr_t = std::unique_ptr<array_t>;
    using table_ptr_t = std::unique_ptr<table_t>;

    float_t _as_float() const;
    integer_t _as_integer() const;

    void _set_string(string_t&& string);

    void _set_float(float_t number);
    void _set_integer(integer_t number);

    std::variant<
        none_t,
        string_t,
        integer_t,
        float_t,
        boolean_t,
        offset_date_time_t,
        local_date_time_t,
        local_date_t,
        local_time_t,
        array_ptr_t,
        table_ptr_t
    > _value;

}; // class Value



} // namespace dl7::toml

#endif // DL7_TOML_VALUE_H
