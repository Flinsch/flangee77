#include "Node.h"

#include "./Element.h"
#include "./Text.h"



namespace dl7::xml {



    bool Node::operator==(const Node& other) const noexcept
    {
        if (get_type() != other.get_type())
            return false;

        switch (get_type())
        {
        case Type::Element:
            return *reinterpret_cast<const Element*>(this) == *reinterpret_cast<const Element*>(&other);
        case Type::Text:
            return *reinterpret_cast<const Text*>(this) == *reinterpret_cast<const Text*>(&other);
        default:
            assert(false);
        }

        return false;
    }



} // namespace dl7::xml
