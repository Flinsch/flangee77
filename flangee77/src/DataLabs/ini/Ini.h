#ifndef DL7_INI_INI_H
#define DL7_INI_INI_H

#include "./Section.h"



namespace dl7::ini {



class Ini
    : public Section
{

public:
    Ini() = default;

    template <typename Tsections>
        requires(std::constructible_from<sections_t, Tsections>)
    Ini(Tsections&& sections)
        : Section(std::forward<Tsections>(sections))
    {
    }

    template <typename Tproperties>
        requires(std::constructible_from<properties_t, Tproperties>)
    Ini(Tproperties&& properties)
        : Section(std::forward<Tproperties>(properties))
    {
    }

    template <typename Tsections, typename Tproperties>
        requires(std::constructible_from<sections_t, Tsections> && std::constructible_from<properties_t, Tproperties>)
    Ini(Tsections&& sections, Tproperties&& properties)
        : Section(std::forward<Tsections>(sections), std::forward<Tproperties>(properties))
    {
    }

    Ini(const Ini& other) = default;
    Ini& operator=(const Ini& other) = default;
    Ini(Ini&& other) noexcept = default;
    Ini& operator=(Ini&& other) noexcept = default;

    ~Ini() noexcept = default;

    void swap(Ini& other) noexcept;



    bool operator==(const Ini& other) const = default;
    bool operator!=(const Ini& other) const { return !(*this == other); }

}; // class Ini



} // namespace dl7::ini

#endif // DL7_INI_INI_H
