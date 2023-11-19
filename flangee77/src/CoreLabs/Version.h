#pragma once
#ifndef CL7_VERSION_H
#define CL7_VERSION_H

#include <CoreLabs/string.h>



namespace cl7 {



struct Version
{

    // #############################################################################
    // Attributes
    // #############################################################################
public:
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

    cl7::string build;



    // #############################################################################
    // Methods
    // #############################################################################
public:
    /**
     * Parses the given version string.
     */
    static Version parse(const cl7::string& string);

    /**
     * "Stringifies" this version object.
     */
    cl7::string to_string(bool short_format = false) const;



    // #############################################################################
    // Comparison Operator
    // #############################################################################
public:
    /**
     * Compares two version objects.
     * The build metadata doesn't actually need to be part of the comparison. But I
     * ask myself: why not? So let's just fall back to the default implementation of
     * the "spaceship" operator.
     */
    auto operator <=> (const Version& rhs) const = default;

}; // struct Version



    template <> inline
    cl7::string to_string(const Version& version) { return version.to_string(); }



} // namespace cl7

#endif // CL7_VERSION_H
