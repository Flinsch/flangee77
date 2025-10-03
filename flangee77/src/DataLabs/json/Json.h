#ifndef DL7_JSON_JSON_H
#define DL7_JSON_JSON_H

#include "./types.h"

#include <variant>



namespace dl7::json {



class Json
{

public:
    enum struct Type
    {
        Null,
        Object,
        Array,
        String,
        Float,
        Integer,
        Boolean,
    };



    Json();
    explicit Json(null_t);
    explicit Json(object_t object);
    explicit Json(array_t array);
    explicit Json(string_t string);
    explicit Json(std::basic_string_view<string_t::value_type> string);
    explicit Json(const string_t::value_type* string);

    explicit Json(float number);
    explicit Json(double number);
    explicit Json(long double number);

    explicit Json(signed number);
    explicit Json(signed long number);
    explicit Json(signed long long number);

    explicit Json(unsigned number);
    explicit Json(unsigned long number);
    explicit Json(unsigned long long number);

    explicit Json(boolean_t boolean);

    explicit Json(Type type);

    Json(const Json& other);
    Json& operator=(const Json& other);
    Json(Json&& other) noexcept;
    Json& operator=(Json&& other) noexcept;

    ~Json() noexcept = default;

    void swap(Json& other) noexcept;



    Type get_type() const noexcept { return static_cast<Type>(_value.index()); }

    bool is_null() const noexcept { return std::holds_alternative<null_t>(_value); }
    bool is_object() const noexcept { return std::holds_alternative<object_ptr_t>(_value); }
    bool is_array() const noexcept { return std::holds_alternative<array_ptr_t>(_value); }
    bool is_string() const noexcept { return std::holds_alternative<string_t>(_value); }
    bool is_float() const noexcept { return std::holds_alternative<float_t>(_value); }
    bool is_integer() const noexcept { return std::holds_alternative<integer_t>(_value); }
    bool is_number() const noexcept { return is_float() || is_integer(); }
    bool is_boolean() const noexcept { return std::holds_alternative<boolean_t>(_value); }
    bool is_true() const noexcept { const boolean_t* b = std::get_if<boolean_t>(&_value); return b ? *b : false; }
    bool is_false() const noexcept { const boolean_t* b = std::get_if<boolean_t>(&_value); return b ? !*b : false; }
    bool is_primitive() const noexcept { return is_null() || is_string() || is_number() || is_boolean(); }
    bool is_structured() const noexcept { return is_object() || is_array(); }

    /**
     * Returns true if this JSON value represents null, an empty object, an empty
     * array, or an empty string; returns false otherwise (i.e., numbers, including
     * 0, and booleans, including false, are considered non-empty).
     */
    bool is_empty() const noexcept;



    const object_t& as_object() const;
    object_t& as_object();

    const array_t& as_array() const;
    array_t& as_array();

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



    void set_null();
    void set_object(object_t object);
    void set_array(array_t array);
    void set_string(string_t string);
    void set_string(std::basic_string_view<string_t::value_type> string);
    void set_string(const string_t::value_type* string);

    template <typename Tnumber>
        requires(std::is_nothrow_convertible_v<Tnumber, float_t>)
    void set_float(Tnumber number) { _set_float(static_cast<float_t>(number)); }

    template <typename Tnumber>
        requires(std::is_nothrow_convertible_v<Tnumber, integer_t>)
    void set_integer(Tnumber number) { _set_integer(static_cast<integer_t>(number)); }

    void set_boolean(boolean_t boolean);

    void reset_type(Type type);



    /**
     * Serializes this JSON value into a compact single-line string representation.
     */
    string_t to_string() const;



    const Json& at(size_t index) const;
    Json& at(size_t index);

    const Json& at(cl7::u8string_view key) const;
    Json& at(cl7::u8string_view key);



    Json& operator[](size_t index);

    Json& operator[](const cl7::u8string& key);
    Json& operator[](cl7::u8string&& key);



    bool operator==(const Json& other) const;
    bool operator!=(const Json& other) const { return !(*this == other); }



private:
    using object_ptr_t = std::unique_ptr<object_t>;
    using array_ptr_t = std::unique_ptr<array_t>;

    float_t _as_float() const;
    integer_t _as_integer() const;

    void _set_float(float_t number);
    void _set_integer(integer_t number);

    std::variant<null_t, object_ptr_t, array_ptr_t, string_t, float_t, integer_t, boolean_t> _value;

}; // class Json



} // namespace dl7::json

#endif // DL7_JSON_JSON_H
