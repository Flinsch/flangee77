#include "Section.h"



namespace dl7::ini {



    Section::Section(const Section& other)
    {
        *_sections = *other._sections;
        *_properties = *other._properties;
    }

    Section& Section::operator=(const Section& other)
    {
        if (this != &other)
        {
            *_sections = *other._sections;
            *_properties = *other._properties;
        }

        return *this;
    }

    Section::Section(Section&& other) noexcept
        : _sections(std::move(other._sections))
        , _properties(std::move(other._properties))
    {
    }

    Section& Section::operator=(Section&& other) noexcept
    {
        _sections = std::move(other._sections);
        _properties = std::move(other._properties);
        return *this;
    }

    void Section::swap(Section& other) noexcept
    {
        _sections.swap(other._sections);
        _properties.swap(other._properties);
    }



    bool Section::operator==(const Section& other) const
    {
        return *_sections == *other._sections && *_properties == *other._properties;
    }



} // namespace dl7::ini
