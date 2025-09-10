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

    template <typename Tsections>
        requires(std::constructible_from<sections_t, Tsections>)
    Section(Tsections&& sections)
        : _sections(std::make_unique<sections_t>(std::forward<Tsections>(sections)))
    {
    }

    template <typename Tproperties>
        requires(std::constructible_from<properties_t, Tproperties>)
    Section(Tproperties&& properties)
        : _properties(std::make_unique<properties_t>(std::forward<Tproperties>(properties)))
    {
    }

    template <typename Tsections, typename Tproperties>
        requires(std::constructible_from<sections_t, Tsections> && std::constructible_from<properties_t, Tproperties>)
    Section(Tsections&& sections, Tproperties&& properties)
        : _sections(std::make_unique<sections_t>(std::forward<Tsections>(sections)))
        , _properties(std::make_unique<properties_t>(std::forward<Tproperties>(properties)))
    {
    }

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



    bool operator==(const Section& other) const;
    bool operator!=(const Section& other) const { return !(*this == other); }



private:
    using sections_ptr_t = std::unique_ptr<sections_t>;
    using properties_ptr_t = std::unique_ptr<properties_t>;

    sections_ptr_t _sections = std::make_unique<sections_t>();
    properties_ptr_t _properties = std::make_unique<properties_t>();

}; // class Section



} // namespace dl7::ini

#endif // DL7_INI_SECTION_H
