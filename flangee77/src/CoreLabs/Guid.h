#pragma once
#ifndef CL7_GUID_H
#define CL7_GUID_H

#include <CoreLabs/string.h>

#include <array>



namespace cl7 {



struct Guid
{

    std::array<std::byte, 16> bytes;


    Guid();
    explicit Guid(const std::array<std::byte, 16>& bytes);
    explicit Guid(cl7::string_view string);

    void swap(Guid& other) noexcept;


    /**
     * Returns true if the GUID does not match 00000000-0000-0000-0000-000000000000.
     */
    bool is_valid() const;

    /**
     * "Stringifies" this GUID object.
     */
    cl7::string to_string(bool uppercase = false) const;


    /**
     * Compares two GUID objects.
     */
    auto operator<=>(const Guid& other) const noexcept = default;


    /**
     * Generates a new GUID.
     */
    static Guid generate();

    /**
     * Parses the given GUID string.
     */
    static Guid parse(cl7::string_view string);

}; // struct Guid



} // namespace cl7

#endif // CL7_GUID_H
