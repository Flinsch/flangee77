#ifndef DL7_CONFIG_JSON_H
#define DL7_CONFIG_JSON_H

#include "./Value.h"

#include <DataLabs/json/Json.h>



namespace dl7::config {



    /**
     * Converts a JSON value into a configuration value. JSON's types are exactly
     * the ones a configuration value has, so nothing is lost and nothing can go
     * wrong, which is why there is nothing to report either.
     */
    Value from_json(const json::Json& json);

    /**
     * Converts a configuration value into a JSON value, likewise without loss.
     */
    json::Json to_json(const Value& value);



} // namespace dl7::config

#endif // DL7_CONFIG_JSON_H
