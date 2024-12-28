#include "Version.h"

#include <CoreLabs/sstream.h>



namespace cl7 {



    /**
     * "Stringifies" this version object.
     */
    cl7::u8string Version::to_string(bool short_format) const
    {
        cl7::u8osstream oss;
        oss << major << '.' << minor;
        if (patch)
            oss << '.' << patch;

        if (pre_release_type != PreReleaseType::None)
        {
            if (short_format)
            {
                switch (pre_release_type)
                {
                    case PreReleaseType::Alpha:
                        oss << 'a';
                        break;
                    case PreReleaseType::Beta:
                        oss << 'b';
                        break;
                    case PreReleaseType::ReleaseCandidate:
                        oss << "rc";
                        break;
                    default:
                        assert(false);
                }
                if (pre_release_number)
                    oss << pre_release_number;
            }
            else // => long/default format
            {
                oss << '-';
                switch (pre_release_type)
                {
                    case PreReleaseType::Alpha:
                        oss << "alpha";
                        break;
                    case PreReleaseType::Beta:
                        oss << "beta";
                        break;
                    case PreReleaseType::ReleaseCandidate:
                        oss << "rc";
                        break;
                    default:
                        assert(false);
                }
                if (pre_release_number)
                    oss << '.' << pre_release_number;
            }
        }

        if (!build.empty())
            oss << (short_format ? ' ' : '+') << build;

        return oss.str();
    }


    /**
     * Parses the given version string.
     */
    Version Version::parse(const cl7::u8string& string)
    {
        Version version;

        cl7::u8isstream iss(string);
        iss >> version.major;
        if (iss.peek() == u8'.')
        {
            iss.ignore(1); // Skip dot
            iss >> version.minor;
        }
        if (iss.peek() == u8'.')
        {
            iss.ignore(1); // Skip dot
            iss >> version.patch;
        }

        cl7::u8char_type peek = iss.peek();
        if (peek == u8'-')
        {
            iss.ignore(1); // Skip hyphen
            cl7::u8string pre_release_identifier;
            std::getline(iss, pre_release_identifier, u8'.');

            if (pre_release_identifier == u8"alpha")
                version.pre_release_type = PreReleaseType::Alpha;
            else if (pre_release_identifier == u8"beta")
                version.pre_release_type = PreReleaseType::Beta;
            else if (pre_release_identifier == u8"rc")
                version.pre_release_type = PreReleaseType::ReleaseCandidate;

            if (version.pre_release_type != PreReleaseType::None) // Dot already skipped by std::getline
                iss >> version.pre_release_number;
        }
        else if (peek == u8'a' || peek == u8'b' || peek == u8'r')
        {
            switch (peek)
            {
                case u8'a':
                    iss.ignore(1); // Skip 'a'
                    version.pre_release_type = PreReleaseType::Alpha;
                    break;
                case u8'b':
                    iss.ignore(1); // Skip 'b'
                    version.pre_release_type = PreReleaseType::Beta;
                    break;
                case u8'r':
                    iss.ignore(1); // Skip 'r'
                    if (iss.peek() == u8'c')
                    {
                        iss.ignore(1); // Skip 'c'
                        version.pre_release_type = PreReleaseType::ReleaseCandidate;
                    }
                    break;
            }

            if (version.pre_release_type != PreReleaseType::None && iss.peek() != u8' ')
                iss >> version.pre_release_number;
        }

        if (iss.peek() == u8'+' || iss.peek() == u8' ')
        {
            iss.ignore(1); // Skip plus/space
            std::getline(iss, version.build, u8' ');
        }

        return version;
    }



} // namespace cl7
