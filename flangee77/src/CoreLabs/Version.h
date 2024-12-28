#pragma once
#ifndef CL7_VERSION_H
#define CL7_VERSION_H

#include <CoreLabs/string.h>



namespace cl7 {



struct Version
{

    unsigned major = 0;
    unsigned minor = 0;
    unsigned patch = 0;

    enum class PreReleaseType : signed
    {
        Alpha = -3,
        Beta = -2,
        ReleaseCandidate = -1,
        Release = 0,
        None = Release,
    } pre_release_type = PreReleaseType::None;

    unsigned pre_release_number = 0;

    cl7::u8string build;



    /**
     * "Stringifies" this version object.
     */
    cl7::u8string to_string(bool short_format = false) const;


    /**
     * Compares two version objects.
     * The build metadata doesn't actually need to be part of the comparison. But I
     * ask myself: why not? So let's just fall back to the default implementation of
     * the "spaceship" operator.
     */
    auto operator<=>(const Version& rhs) const noexcept = default;


    /**
     * Parses the given version string.
     */
    static Version parse(const cl7::u8string& string);

}; // struct Version



} // namespace cl7

#endif // CL7_VERSION_H
