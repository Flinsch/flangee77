#ifndef DL7_JSON_JSON_H
#define DL7_JSON_JSON_H

#include "./types.h"

#include <variant>



namespace dl7::json {



class json
{

public:
    enum class type
    {
        null,
        object,
        array,
        string,
        number_real,
        number_integer,
        number_unsigned,
        boolean,
    };



    type get_type() const noexcept { return static_cast<type>(_value.index()); }

    bool is_null() const noexcept { return std::holds_alternative<null_t>(_value); }
    bool is_object() const noexcept { return std::holds_alternative<object_t>(_value); }
    bool is_array() const noexcept { return std::holds_alternative<array_t>(_value); }
    bool is_string() const noexcept { return std::holds_alternative<string_t>(_value); }
    bool is_number_real() const noexcept { return std::holds_alternative<number_real_t>(_value); }
    bool is_number_integer() const noexcept { return std::holds_alternative<number_integer_t>(_value); }
    bool is_number_unsigned() const noexcept { return std::holds_alternative<number_unsigned_t>(_value); }
    bool is_number() const noexcept { return is_number_real() || is_number_integer() || is_number_unsigned(); }
    bool is_boolean() const noexcept { return std::holds_alternative<boolean_t>(_value); }
    bool is_true() const noexcept { return is_boolean() && std::get<boolean_t>(_value); }
    bool is_false() const noexcept { return is_boolean() && !std::get<boolean_t>(_value); }
    bool is_primitive() const noexcept { return is_null() || is_string() || is_number() || is_boolean(); }

    const object_t& as_object() const;
    object_t& as_object();

    const array_t& as_array() const;
    array_t& as_array();

    const string_t& as_string() const;
    string_t& as_string();

    number_real_t as_number_real() const;
    number_integer_t as_number_integer() const;
    number_unsigned_t as_number_unsigned() const;
    boolean_t as_boolean() const;

    void set_null();
    void set_object(object_t object);
    void set_array(array_t array);
    void set_string(string_t string);
    void set_number_real(number_real_t number);
    void set_number_integer(number_integer_t number);
    void set_number_unsigned(number_unsigned_t number);
    void set_boolean(boolean_t boolean);



private:
    std::variant<null_t, object_t, array_t, string_t, number_real_t, number_integer_t, number_unsigned_t, boolean_t> _value;

}; // class json



} // namespace dl7::json

#endif // DL7_JSON_JSON_H
