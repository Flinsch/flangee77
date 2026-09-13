#include "json.h"

#include <utility>



namespace dl7::config {



    /**
     * Converts a JSON value into a configuration value. JSON's types are exactly
     * the ones a configuration value has, so nothing is lost and nothing can go
     * wrong, which is why there is nothing to report either.
     */
    Value from_json(const json::Json& json)
    {
        switch (json.get_type())
        {
        case json::Json::Type::Null:
            return {};
        case json::Json::Type::Boolean:
            return Value{json.as_boolean()};
        case json::Json::Type::Integer:
            return Value{json.as_integer()};
        case json::Json::Type::Float:
            return Value{json.as_float()};
        case json::Json::Type::String:
            return Value{json.as_string()};

        case json::Json::Type::Array:
        {
            sequence_t sequence;
            sequence.reserve(json.as_array().size());

            for (const auto& entry : json.as_array())
                sequence.push_back(from_json(entry));

            return Value{std::move(sequence)};
        }

        case json::Json::Type::Object:
        {
            mapping_t mapping;

            for (const auto& p : json.as_object())
                mapping[p.first] = from_json(p.second);

            return Value{std::move(mapping)};
        }
        }

        assert(false);
        return {};
    }

    /**
     * Converts a configuration value into a JSON value, likewise without loss.
     */
    json::Json to_json(const Value& value)
    {
        switch (value.get_type())
        {
        case Value::Type::Null:
            return json::Json{};
        case Value::Type::Boolean:
            return json::Json{value.as_boolean()};
        case Value::Type::Integer:
            return json::Json{value.as_integer()};
        case Value::Type::Float:
            return json::Json{value.as_float()};
        case Value::Type::String:
            return json::Json{value.as_string()};

        case Value::Type::Sequence:
        {
            json::array_t array;
            array.reserve(value.as_sequence().size());

            for (const auto& entry : value.as_sequence())
                array.push_back(to_json(entry));

            return json::Json{std::move(array)};
        }

        case Value::Type::Mapping:
        {
            json::object_t object;

            for (const auto& p : value.as_mapping())
                object[p.first] = to_json(p.second);

            return json::Json{std::move(object)};
        }
        }

        assert(false);
        return json::Json{};
    }



} // namespace dl7::config
