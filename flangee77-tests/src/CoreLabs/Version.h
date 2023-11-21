#pragma once
#ifndef F77_TESTS_CL7_VERSION_H
#define F77_TESTS_CL7_VERSION_H

#include <CoreLabs/Version.h>

#include <TestLabs/TestSuite.h>

#include <vector>



namespace cl7 {
    template <> inline
    cl7::string to_string(const Version& version) { return TEXT("{\"") + version.to_string() + TEXT("\"}"); }
}



TESTLABS_CASE( TEXT("CoreLabs:  Version:  to_string/parse") )
{
    struct Entry
    {
        cl7::Version version;
        cl7::string default_format;
        cl7::string short_format;
    } entry;

    const std::vector<Entry> container {
        { cl7::Version{}, TEXT("0.0"), TEXT("0.0") },
        { cl7::Version{ 0, 9 }, TEXT("0.9"), TEXT("0.9") },
        { cl7::Version{ 0, 9, 1 }, TEXT("0.9.1"), TEXT("0.9.1") },
        { cl7::Version{ 1, 0 }, TEXT("1.0"), TEXT("1.0") },
        { cl7::Version{ 1, 2 }, TEXT("1.2"), TEXT("1.2") },
        { cl7::Version{ 1, 2, 3 }, TEXT("1.2.3"), TEXT("1.2.3") },
        { cl7::Version{ 0, 9, 0, cl7::Version::PreReleaseType::Alpha }, TEXT("0.9-alpha"), TEXT("0.9a") },
        { cl7::Version{ 0, 9, 0, cl7::Version::PreReleaseType::Beta }, TEXT("0.9-beta"), TEXT("0.9b") },
        { cl7::Version{ 0, 9, 0, cl7::Version::PreReleaseType::ReleaseCandidate }, TEXT("0.9-rc"), TEXT("0.9rc") },
        { cl7::Version{ 0, 9, 1, cl7::Version::PreReleaseType::Alpha }, TEXT("0.9.1-alpha"), TEXT("0.9.1a") },
        { cl7::Version{ 0, 9, 1, cl7::Version::PreReleaseType::Beta }, TEXT("0.9.1-beta"), TEXT("0.9.1b") },
        { cl7::Version{ 0, 9, 1, cl7::Version::PreReleaseType::ReleaseCandidate }, TEXT("0.9.1-rc"), TEXT("0.9.1rc") },
        { cl7::Version{ 0, 9, 1, cl7::Version::PreReleaseType::Alpha, 2 }, TEXT("0.9.1-alpha.2"), TEXT("0.9.1a2") },
        { cl7::Version{ 0, 9, 1, cl7::Version::PreReleaseType::Beta, 2 }, TEXT("0.9.1-beta.2"), TEXT("0.9.1b2") },
        { cl7::Version{ 0, 9, 1, cl7::Version::PreReleaseType::ReleaseCandidate, 2 }, TEXT("0.9.1-rc.2"), TEXT("0.9.1rc2") },
        { cl7::Version{ 1, 0, 0, cl7::Version::PreReleaseType::None, 0, TEXT("build") }, TEXT("1.0+build"), TEXT("1.0 build") },
        { cl7::Version{ 1, 2, 3, cl7::Version::PreReleaseType::ReleaseCandidate, 4, TEXT("build") }, TEXT("1.2.3-rc.4+build"), TEXT("1.2.3rc4 build") },
        { cl7::Version{ 1, 22 }, TEXT("1.22"), TEXT("1.22") },
        { cl7::Version{ 1, 22, 3 }, TEXT("1.22.3"), TEXT("1.22.3") },
    };

    for ( size_t i = 0; i < container.size(); ++i )
    {
        const Entry& entry = container[ i ];

        TESTLABS_CHECK_EQ( entry.version.to_string( false ), entry.default_format );
        TESTLABS_CHECK_EQ( entry.version.to_string( true ), entry.short_format );
        TESTLABS_CHECK_EQ( cl7::Version::parse( entry.default_format ), entry.version );
        TESTLABS_CHECK_EQ( cl7::Version::parse( entry.short_format ), entry.version );
    }
}

TESTLABS_CASE( TEXT("CoreLabs:  Version:  parsing bad strings") )
{
    struct Entry
    {
        cl7::string string;
        cl7::Version version;
    } entry;

    const std::vector<Entry> container {
        { TEXT(""), cl7::Version{} },
        { TEXT("0"), cl7::Version{ 0 } },
        { TEXT("1"), cl7::Version{ 1 } },
        { TEXT("foo"), cl7::Version{} },
        { TEXT("1.0-bad"), cl7::Version{ 1, 0 } },
        { TEXT("1.0-bad.2"), cl7::Version{ 1, 0 } },
        { TEXT("1.0-bad.3+build"), cl7::Version{ 1, 0 } },
        { TEXT("1.0bad"), cl7::Version{ 1, 0, 0, cl7::Version::PreReleaseType::Beta } },
        { TEXT("1.0bad2"), cl7::Version{ 1, 0, 0, cl7::Version::PreReleaseType::Beta } },
        { TEXT("1.0bad3 build"), cl7::Version{ 1, 0, 0, cl7::Version::PreReleaseType::Beta } },
        { TEXT("1-rc.1"), cl7::Version{ 1, 0, 0, cl7::Version::PreReleaseType::ReleaseCandidate, 1 } },
    };

    for ( size_t i = 0; i < container.size(); ++i )
    {
        const Entry& entry = container[ i ];

        TESTLABS_CHECK_EQ( cl7::Version::parse( entry.string ), entry.version );
    }
}

TESTLABS_CASE( TEXT("CoreLabs:  Version:  comparisons") )
{
    const std::vector<cl7::Version> container {
        { cl7::Version{ 0, 9 } },
        { cl7::Version{ 1, 0, 0, cl7::Version::PreReleaseType::Alpha } },
        { cl7::Version{ 1, 0, 0, cl7::Version::PreReleaseType::Alpha, 1 } },
        { cl7::Version{ 1, 0, 0, cl7::Version::PreReleaseType::Alpha, 2 } },
        { cl7::Version{ 1, 0, 0, cl7::Version::PreReleaseType::Alpha, 11 } },
        { cl7::Version{ 1, 0, 0, cl7::Version::PreReleaseType::Beta, 0 } },
        { cl7::Version{ 1, 0, 0, cl7::Version::PreReleaseType::Beta, 1 } },
        { cl7::Version{ 1, 0, 0, cl7::Version::PreReleaseType::ReleaseCandidate, 0 } },
        { cl7::Version{ 1, 0, 0, cl7::Version::PreReleaseType::ReleaseCandidate, 1 } },
        { cl7::Version{ 1, 0, 0 } },
        { cl7::Version{ 1, 0 } },
        { cl7::Version{ 1, 0, 0 } },
        { cl7::Version{ 1, 0, 1 } },
        { cl7::Version{ 1, 1 } },
        { cl7::Version{ 2, 0 } },
        { cl7::Version{ 2, 1 } },
        { cl7::Version{ 2, 2 } },
        { cl7::Version{ 2, 11 } },
        { cl7::Version{ 11, 0 } },
    };

    for ( size_t i = 1; i < container.size(); ++i )
    {
        const cl7::Version& version0 = container[ i - 1 ];
        const cl7::Version& version1 = container[ i ];

        TESTLABS_CHECK_LE( version0, version1 );
    }
}



#endif // F77_TESTS_CL7_VERSION_H
