#include "Attribute.h"



namespace dl7::xml {



    Attribute::Attribute(cl7::u8string name, cl7::u8string value)
        : _name(std::move(name))
        , _value(std::move(value))
    {
    }

    void Attribute::swap(Attribute& other) noexcept
    {
        _name.swap(other._name);
        _value.swap(other._value);
    }



} // namespace dl7::xml
