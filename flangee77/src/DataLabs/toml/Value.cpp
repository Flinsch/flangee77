#include "Value.h"

#include <DataLabs/json/util/Escaper.h>

#include <CoreLabs/text/format.h>
#include <CoreLabs/sstream.h>

#include <chrono>
#include <cmath>
#include <utility>



namespace dl7::toml {



namespace {

    void _append_padded(cl7::u8osstream& oss, long long value, size_t width)
    {
        auto digits = cl7::text::format::to_string<cl7::u8string>(value);
        for (size_t i = digits.size(); i < width; ++i)
            oss << u8'0';
        oss << digits;
    }

    void _write_date(cl7::u8osstream& oss, const std::chrono::year_month_day& ymd)
    {
        _append_padded(oss, static_cast<int>(ymd.year()), 4);
        oss << u8'-';
        _append_padded(oss, static_cast<unsigned>(ymd.month()), 2);
        oss << u8'-';
        _append_padded(oss, static_cast<unsigned>(ymd.day()), 2);
    }

    void _write_time(cl7::u8osstream& oss, const std::chrono::hh_mm_ss<std::chrono::seconds>& hms)
    {
        _append_padded(oss, hms.hours().count(), 2);
        oss << u8':';
        _append_padded(oss, hms.minutes().count(), 2);
        oss << u8':';
        _append_padded(oss, hms.seconds().count(), 2);
    }

} // namespace



    Value::Value()
        : _value(none_t{})
    {
    }

    Value::Value(none_t none)
        : _value(none_t{})
    {
    }

    Value::Value(array_t array)
        : _value(std::make_unique<array_t>(std::move(array)))
    {
    }

    Value::Value(table_t table)
        : _value(std::make_unique<table_t>(std::move(table)))
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

    Value::Value(offset_date_time_t offset_date_time)
        : _value(offset_date_time)
    {
    }

    Value::Value(local_date_time_t local_date_time)
        : _value(local_date_time)
    {
    }

    Value::Value(local_date_t local_date)
        : _value(local_date)
    {
    }

    Value::Value(local_time_t local_time)
        : _value(local_time)
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
        case Type::Array:
            set_array(other.as_array());
            break;
        case Type::Table:
            set_table(other.as_table());
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
        case Type::OffsetDateTime:
            set_offset_date_time(other.as_offset_date_time());
            break;
        case Type::LocalDateTime:
            set_local_date_time(other.as_local_date_time());
            break;
        case Type::LocalDate:
            set_local_date(other.as_local_date());
            break;
        case Type::LocalTime:
            set_local_time(other.as_local_time());
            break;
        default:
            assert(is_none());
            set_none();
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
     * Returns true if this TOML value represents "none", an empty array, an empty
     * table, or an empty string; returns false otherwise (i.e., numbers, including
     * 0, and booleans, including false, are considered non-empty).
     */
    bool Value::is_empty() const noexcept
    {
        if (is_none()) return true;

        if (is_array()) { const array_ptr_t* array_ptr = std::get_if<array_ptr_t>(&_value); return array_ptr ? (*array_ptr)->empty() : true; }
        if (is_table()) { const table_ptr_t* table_ptr = std::get_if<table_ptr_t>(&_value); return table_ptr ? (*table_ptr)->empty() : true; }
        if (is_string()) { const string_t* string = std::get_if<string_t>(&_value); return string ? string->empty() : true; }

        return false;
    }



    const array_t& Value::as_array() const
    {
        assert(is_array());
        return *std::get<array_ptr_t>(_value);
    }

    array_t& Value::as_array()
    {
        return const_cast<array_t&>(std::as_const(*this).as_array()); // NOLINT(cppcoreguidelines-pro-type-const-cast)
    }

    const table_t& Value::as_table() const
    {
        assert(is_table());
        return *std::get<table_ptr_t>(_value);
    }

    table_t& Value::as_table()
    {
        return const_cast<table_t&>(std::as_const(*this).as_table()); // NOLINT(cppcoreguidelines-pro-type-const-cast)
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

    offset_date_time_t Value::as_offset_date_time() const
    {
        assert(is_offset_date_time());
        return std::get<offset_date_time_t>(_value);
    }

    local_date_time_t Value::as_local_date_time() const
    {
        assert(is_local_date_time());
        return std::get<local_date_time_t>(_value);
    }

    local_date_t Value::as_local_date() const
    {
        assert(is_local_date());
        return std::get<local_date_t>(_value);
    }

    local_time_t Value::as_local_time() const
    {
        assert(is_local_time());
        return std::get<local_time_t>(_value);
    }



    void Value::set_none()
    {
        _value = none_t{};
    }

    void Value::set_array(array_t array)
    {
        _value = std::make_unique<array_t>(std::move(array));
    }

    void Value::set_table(table_t table)
    {
        _value = std::make_unique<table_t>(std::move(table));
    }

    void Value::set_boolean(boolean_t boolean)
    {
        _value = boolean;
    }

    void Value::set_offset_date_time(offset_date_time_t offset_date_time)
    {
        _value = offset_date_time;
    }

    void Value::set_local_date_time(local_date_time_t local_date_time)
    {
        _value = local_date_time;
    }

    void Value::set_local_date(local_date_t local_date)
    {
        _value = local_date;
    }

    void Value::set_local_time(local_time_t local_time)
    {
        _value = local_time;
    }

    void Value::reset_type(Type type)
    {
        switch (type)
        {
        case Type::None:
            set_none();
            break;
        case Type::Array:
            set_array({});
            break;
        case Type::Table:
            set_table({});
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
        case Type::OffsetDateTime:
            set_offset_date_time(offset_date_time_t{});
            break;
        case Type::LocalDateTime:
            set_local_date_time(local_date_time_t{});
            break;
        case Type::LocalDate:
            set_local_date(local_date_t{});
            break;
        case Type::LocalTime:
            set_local_time(local_time_t{std::chrono::seconds{0}});
            break;
        default:
            assert(false);
            set_none();
        }
    }



    /**
     * Serializes this TOML value into an inline string representation.
     */
    string_t Value::to_string() const
    {
        cl7::u8osstream oss;

        switch (get_type())
        {
        case Type::None:
            break;

        case Type::String:
            oss << u8'"';
            oss << json::util::Escaper{}.escape_string(as_string());
            oss << u8'"';
            break;

        case Type::Integer:
            oss << cl7::text::format::to_string<string_t>(as_integer());
            break;

        case Type::Float:
        {
            const float_t number = as_float();
            if (std::isnan(number))
                oss << u8"nan";
            else if (std::isinf(number))
                oss << (number < 0.0 ? u8"-inf" : u8"inf");
            else
                oss << cl7::text::format::to_string<string_t>(number, 1);
            break;
        }

        case Type::Boolean:
            oss << (as_boolean() ? u8"true" : u8"false");
            break;

        case Type::OffsetDateTime:
        {
            const auto [time_point, offset_minutes] = as_offset_date_time();
            // The instant is stored as UTC; shift it back to the original offset's
            // wall-clock time for display.
            const auto local = time_point + std::chrono::minutes{offset_minutes};
            const auto days = std::chrono::floor<std::chrono::days>(local);
            _write_date(oss, std::chrono::year_month_day{std::chrono::sys_days{days.time_since_epoch()}});
            oss << u8'T';
            _write_time(oss, std::chrono::hh_mm_ss<std::chrono::seconds>{local - days});
            if (offset_minutes == 0)
            {
                oss << u8'Z';
            }
            else
            {
                oss << (offset_minutes < 0 ? u8'-' : u8'+');
                const int abs_minutes = offset_minutes < 0 ? -offset_minutes : offset_minutes;
                _append_padded(oss, abs_minutes / 60, 2);
                oss << u8':';
                _append_padded(oss, abs_minutes % 60, 2);
            }
            break;
        }

        case Type::LocalDateTime:
        {
            const auto time_point = as_local_date_time();
            const auto days = std::chrono::floor<std::chrono::days>(time_point);
            _write_date(oss, std::chrono::year_month_day{std::chrono::sys_days{days.time_since_epoch()}});
            oss << u8'T';
            _write_time(oss, std::chrono::hh_mm_ss<std::chrono::seconds>{time_point - days});
            break;
        }

        case Type::LocalDate:
            _write_date(oss, as_local_date());
            break;

        case Type::LocalTime:
            _write_time(oss, as_local_time());
            break;

        case Type::Array:
        {
            const array_t& array = as_array();
            oss << u8'[';
            for (size_t i = 0; i < array.size(); ++i)
            {
                if (i > 0)
                    oss << u8", ";
                oss << array[i].to_string();
            }
            oss << u8']';
            break;
        }

        case Type::Table:
        {
            const table_t& table = as_table();
            oss << u8'{';
            size_t i = 0;
            for (const auto& p : table)
            {
                if (i++ > 0)
                    oss << u8", ";
                oss << p.first << u8" = " << p.second.to_string();
            }
            oss << u8'}';
            break;
        }
        }

        return std::move(oss).str();
    }



    const Value& Value::at(size_t index) const
    {
        return as_array().at(index);
    }

    Value& Value::at(size_t index)
    {
        return as_array().at(index);
    }

    const Value& Value::at(cl7::u8string_view key) const
    {
        return as_table().at(key);
    }

    Value& Value::at(cl7::u8string_view key)
    {
        return as_table().at(key);
    }



    Value& Value::operator[](size_t index)
    {
        if (is_none())
            set_array({});
        return as_array()[index];
    }

    Value& Value::operator[](const cl7::u8string& key)
    {
        if (is_none())
            set_table({});
        return as_table()[key];
    }

    Value& Value::operator[](cl7::u8string&& key)
    {
        if (is_none())
            set_table({});
        return as_table()[std::move(key)];
    }



    bool Value::operator==(const Value& other) const
    {
        if (get_type() != other.get_type())
            return false;

        switch (get_type())
        {
        case Type::None:
            return true;
        case Type::String:
            return as_string() == other.as_string();
        case Type::Integer:
            return _as_integer() == other._as_integer();
        case Type::Float:
            return _as_float() == other._as_float();
        case Type::Boolean:
            return as_boolean() == other.as_boolean();
        case Type::OffsetDateTime:
            return as_offset_date_time() == other.as_offset_date_time();
        case Type::LocalDateTime:
            return as_local_date_time() == other.as_local_date_time();
        case Type::LocalDate:
            return as_local_date() == other.as_local_date();
        case Type::LocalTime:
            return as_local_time().to_duration() == other.as_local_time().to_duration();
        case Type::Array:
            return as_array() == other.as_array();
        case Type::Table:
            return as_table() == other.as_table();
        }

        return false;
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



} // namespace dl7::toml
