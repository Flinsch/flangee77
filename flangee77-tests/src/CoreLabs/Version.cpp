
#include <TestLabs/TestSuite.h>

#include <CoreLabs/Version.h>

#include "../shared.h"

#include <vector>



namespace tl7::internals {
    inline
    cl7::u8string to_string(const cl7::Version& version) { return u8"{\"" + version.to_string() + u8"\"}"; }
}




TESTLABS_CASE( u8"CoreLabs:  Version:  to_string/parse" )
{
    struct Entry
    {
        cl7::Version version;
        cl7::u8string default_format;
        cl7::u8string short_format;
    } entry;

    const std::vector<Entry> container {
        { cl7::Version{}, u8"0.0", u8"0.0" },
        { cl7::Version{ 0, 9 }, u8"0.9", u8"0.9" },
        { cl7::Version{ 0, 9, 1 }, u8"0.9.1", u8"0.9.1" },
        { cl7::Version{ 1, 0 }, u8"1.0", u8"1.0" },
        { cl7::Version{ 1, 2 }, u8"1.2", u8"1.2" },
        { cl7::Version{ 1, 2, 3 }, u8"1.2.3", u8"1.2.3" },
        { cl7::Version{ 0, 9, 0, cl7::Version::PreReleaseType::Alpha }, u8"0.9-alpha", u8"0.9a" },
        { cl7::Version{ 0, 9, 0, cl7::Version::PreReleaseType::Beta }, u8"0.9-beta", u8"0.9b" },
        { cl7::Version{ 0, 9, 0, cl7::Version::PreReleaseType::ReleaseCandidate }, u8"0.9-rc", u8"0.9rc" },
        { cl7::Version{ 0, 9, 1, cl7::Version::PreReleaseType::Alpha }, u8"0.9.1-alpha", u8"0.9.1a" },
        { cl7::Version{ 0, 9, 1, cl7::Version::PreReleaseType::Beta }, u8"0.9.1-beta", u8"0.9.1b" },
        { cl7::Version{ 0, 9, 1, cl7::Version::PreReleaseType::ReleaseCandidate }, u8"0.9.1-rc", u8"0.9.1rc" },
        { cl7::Version{ 0, 9, 1, cl7::Version::PreReleaseType::Alpha, 2 }, u8"0.9.1-alpha.2", u8"0.9.1a2" },
        { cl7::Version{ 0, 9, 1, cl7::Version::PreReleaseType::Beta, 2 }, u8"0.9.1-beta.2", u8"0.9.1b2" },
        { cl7::Version{ 0, 9, 1, cl7::Version::PreReleaseType::ReleaseCandidate, 2 }, u8"0.9.1-rc.2", u8"0.9.1rc2" },
        { cl7::Version{ 1, 0, 0, cl7::Version::PreReleaseType::None, 0, u8"build" }, u8"1.0+build", u8"1.0 build" },
        { cl7::Version{ 1, 2, 3, cl7::Version::PreReleaseType::ReleaseCandidate, 4, u8"build" }, u8"1.2.3-rc.4+build", u8"1.2.3rc4 build" },
        { cl7::Version{ 1, 22 }, u8"1.22", u8"1.22" },
        { cl7::Version{ 1, 22, 3 }, u8"1.22.3", u8"1.22.3" },
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

TESTLABS_CASE( u8"CoreLabs:  Version:  parsing bad strings" )
{
    struct Entry
    {
        cl7::u8string string;
        cl7::Version version;
    } entry;

    const std::vector<Entry> container {
        { u8"", cl7::Version{} },
        { u8"0", cl7::Version{ 0 } },
        { u8"1", cl7::Version{ 1 } },
        { u8"foo", cl7::Version{} },
        { u8"1.0-bad", cl7::Version{ 1, 0 } },
        { u8"1.0-bad.2", cl7::Version{ 1, 0 } },
        { u8"1.0-bad.3+build", cl7::Version{ 1, 0 } },
        { u8"1.0bad", cl7::Version{ 1, 0, 0, cl7::Version::PreReleaseType::Beta } },
        { u8"1.0bad2", cl7::Version{ 1, 0, 0, cl7::Version::PreReleaseType::Beta } },
        { u8"1.0bad3 build", cl7::Version{ 1, 0, 0, cl7::Version::PreReleaseType::Beta } },
        { u8"1-rc.1", cl7::Version{ 1, 0, 0, cl7::Version::PreReleaseType::ReleaseCandidate, 1 } },
    };

    for ( size_t i = 0; i < container.size(); ++i )
    {
        const Entry& entry = container[ i ];

        TESTLABS_CHECK_EQ( cl7::Version::parse( entry.string ), entry.version );
    }
}

TESTLABS_CASE( u8"CoreLabs:  Version:  comparisons" )
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
