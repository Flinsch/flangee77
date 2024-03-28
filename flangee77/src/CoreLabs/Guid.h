#pragma once
#ifndef CL7_GUID_H
#define CL7_GUID_H

#include <CoreLabs/string.h>

#include <array>



namespace cl7 {



struct Guid
{

    // #############################################################################
    // Construction / Destruction
    // #############################################################################
public:
    /**
     * Default constructor.
     */
    Guid();

    /**
     * Explicit constructor.
     */
    explicit Guid(const std::array<std::byte, 16>& bytes);

    /**
     * Explicit constructor.
     */
    explicit Guid(std::array<std::byte, 16>&& bytes);

    /**
     * Explicit constructor.
     */
    explicit Guid(cl7::string_view string);

    /**
     * Swap operation.
     */
    void swap(Guid& rhs);



    // #############################################################################
    // Attributes
    // #############################################################################
public:
    std::array<std::byte, 16> bytes;



    // #############################################################################
    // Properties
    // #############################################################################
public:
    /**
     * Returns true if the GUID does not match 00000000-0000-0000-0000-000000000000.
     */
    bool is_valid() const;



    // #############################################################################
    // Methods
    // #############################################################################
public:
    /**
     * Generates a new GUID.
     */
    static Guid generate();

    /**
     * Parses the given GUID string.
     */
    static Guid parse(cl7::string_view string);

    /**
     * "Stringifies" this GUID object.
     */
    cl7::string to_string(bool uppercase = false) const;



    // #############################################################################
    // Comparison Operator
    // #############################################################################
public:
    /**
     * Compares two GUID objects.
     */
    auto operator <=> (const Guid& rhs) const = default;

}; // struct Guid



} // namespace cl7

#endif // CL7_GUID_H
