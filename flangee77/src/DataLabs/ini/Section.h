#ifndef DL7_INI_SECTION_H
#define DL7_INI_SECTION_H

#include "./Value.h"

#include <CoreLabs/ordered_map.h>



namespace dl7::ini {



class Section
{

public:
    using sections_t = cl7::ordered_map<cl7::u8string, Section, cl7::u8string_view>;
    using properties_t = cl7::ordered_map<cl7::u8string, Value, cl7::u8string_view>;



    Section() = default;

    Section(const Section& other);
    Section& operator=(const Section& other);
    Section(Section&& other) noexcept;
    Section& operator=(Section&& other) noexcept;

    ~Section() noexcept = default;

    void swap(Section& other) noexcept;



    const sections_t& sections() const { return *_sections; }
    sections_t& sections() { return *_sections; }

    const properties_t& properties() const { return *_properties; }
    properties_t& properties() { return *_properties; }



private:
    using sections_ptr_t = std::unique_ptr<sections_t>;
    using properties_ptr_t = std::unique_ptr<properties_t>;

    sections_ptr_t _sections = std::make_unique<sections_t>();
    properties_ptr_t _properties = std::make_unique<properties_t>();

}; // class Section



} // namespace dl7::ini

#endif // DL7_INI_SECTION_H
