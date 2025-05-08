#ifndef DL7_INI_INI_H
#define DL7_INI_INI_H

#include "./Section.h"



namespace dl7::ini {



class Ini
    : public Section
{

public:
    Ini() = default;

    Ini(const Ini& other) = default;
    Ini& operator=(const Ini& other) = default;
    Ini(Ini&& other) noexcept = default;
    Ini& operator=(Ini&& other) noexcept = default;

    ~Ini() noexcept = default;

    void swap(Ini& other) noexcept;

}; // class Json



} // namespace dl7::ini

#endif // DL7_INI_INI_H
