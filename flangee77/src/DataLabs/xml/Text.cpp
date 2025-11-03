#include "Text.h"



namespace dl7::xml {



    Text::Text(const Text& other, Element* parent_element)
        : Text(other._content, parent_element)
    {
    }

    Text::Text(Text&& other, Element* parent_element) noexcept
        : Text(std::move(other._content), parent_element)
    {
    }



} // namespace dl7::xml
