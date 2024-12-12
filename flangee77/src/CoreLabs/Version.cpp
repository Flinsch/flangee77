#include "Version.h"

#include <CoreLabs/sstream.h>



namespace cl7 {



    /**
     * "Stringifies" this version object.
     */
    cl7::string Version::to_string(bool short_format) const
    {
        cl7::osstream oss;
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
    Version Version::parse(const cl7::string& string)
    {
        Version version;

        cl7::isstream iss(string);
        iss >> version.major;
        if (iss.peek() == TEXT('.'))
        {
            iss.ignore(1); // Skip dot
            iss >> version.minor;
        }
        if (iss.peek() == TEXT('.'))
        {
            iss.ignore(1); // Skip dot
            iss >> version.patch;
        }

        cl7::char_type peek = iss.peek();
        if (peek == TEXT('-'))
        {
            iss.ignore(1); // Skip hyphen
            cl7::string pre_release_identifier;
            std::getline(iss, pre_release_identifier, TEXT('.'));

            if (pre_release_identifier == TEXT("alpha"))
                version.pre_release_type = PreReleaseType::Alpha;
            else if (pre_release_identifier == TEXT("beta"))
                version.pre_release_type = PreReleaseType::Beta;
            else if (pre_release_identifier == TEXT("rc"))
                version.pre_release_type = PreReleaseType::ReleaseCandidate;

            if (version.pre_release_type != PreReleaseType::None) // Dot already skipped by std::getline
                iss >> version.pre_release_number;
        }
        else if (peek == TEXT('a') || peek == TEXT('b') || peek == TEXT('r'))
        {
            switch (peek)
            {
                case TEXT('a'):
                    iss.ignore(1); // Skip 'a'
                    version.pre_release_type = PreReleaseType::Alpha;
                    break;
                case TEXT('b'):
                    iss.ignore(1); // Skip 'b'
                    version.pre_release_type = PreReleaseType::Beta;
                    break;
                case TEXT('r'):
                    iss.ignore(1); // Skip 'r'
                    if (iss.peek() == TEXT('c'))
                    {
                        iss.ignore(1); // Skip 'c'
                        version.pre_release_type = PreReleaseType::ReleaseCandidate;
                    }
                    break;
            }

            if (version.pre_release_type != PreReleaseType::None && iss.peek() != TEXT(' '))
                iss >> version.pre_release_number;
        }

        if (iss.peek() == TEXT('+') || iss.peek() == TEXT(' '))
        {
            iss.ignore(1); // Skip plus/space
            std::getline(iss, version.build, TEXT(' '));
        }

        return version;
    }



} // namespace cl7
