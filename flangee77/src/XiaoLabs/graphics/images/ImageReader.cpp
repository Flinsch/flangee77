#include "ImageReader.h"

#include <CoreLabs/io/File.h>
#include <CoreLabs/logging.h>



namespace xl7::graphics::images {



    /**
     * Loads an image from a file.
     */
    bool ImageReader::load_from_file(const cl7::u8string& file_path, Image& image)
    {
        cl7::io::File file(file_path, cl7::io::OpenMode::Read);
        if (!file.is_readable())
        {
            LOG_ERROR(u8"The image file \"" + file_path + u8"\" is not readable. Does it exist?");
            return false;
        }

        return load_from(file, file_path, image);
    }

    /**
     * Loads an image from any readable object.
     */
    bool ImageReader::load_from(cl7::io::IReadable& readable, const cl7::u8string& source_name, Image& image)
    {
        if (!readable.is_readable())
        {
            LOG_ERROR(u8"The image source \"" + source_name + u8"\" can not be processed.");
            return false;
        }

        if (!_load_from(readable, source_name, image))
        {
            LOG_ERROR(u8"The image could not be loaded from \"" + source_name + u8"\".");
            return false;
        }

        return true;
    }



    static bool _log_error(const cl7::u8string& base_message, const cl7::u8string& description)
    {
        if (description.empty())
            LOG_ERROR(base_message + u8".");
        else
            LOG_ERROR(base_message + u8": " + description);
        return false;
    }

    bool ImageReader::_log_unknown_format_error(const cl7::u8string& source_name, const cl7::u8string& description)
    {
        return _log_error(u8"The format of image \"" + source_name + u8"\" is invalid/unknown", description);
    }

    bool ImageReader::_log_unsupported_format_error(const cl7::u8string& source_name, const cl7::u8string& description)
    {
        return _log_error(u8"The specific (sub)format of image \"" + source_name + u8"\" is not supported", description);
    }

    bool ImageReader::_log_bad_header_error(const cl7::u8string& source_name, const cl7::u8string& description)
    {
        return _log_error(u8"Bad header of image \"" + source_name + u8"\" is damaged", description);
    }

    bool ImageReader::_log_bad_data_error(const cl7::u8string& source_name, const cl7::u8string& description)
    {
        return _log_error(u8"Bad data of image \"" + source_name + u8"\" is damaged", description);
    }



} // namespace xl7::graphics::images
