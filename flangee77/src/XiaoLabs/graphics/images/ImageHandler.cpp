#include "ImageHandler.h"

#include <CoreLabs/io/file.h>
#include <CoreLabs/logging.h>



namespace xl7::graphics::images {



    /**
     * Loads an image from a file.
     */
    bool ImageHandler::load_from_file(const cl7::string& file_path, Image& image)
    {
        cl7::io::file file(file_path, cl7::io::open_mode::read);
        if (!file.is_readable())
        {
            LOG_ERROR(TEXT("The image file \"") + file_path + TEXT("\" is not readable. Does it exist?"));
            return false;
        }

        return load_from(file, file_path, image);
    }

    /**
     * Loads an image from any rom.
     */
    bool ImageHandler::load_from(cl7::io::irom& rom, const cl7::string& rom_name, Image& image)
    {
        if (!rom.is_good())
        {
            LOG_ERROR(TEXT("The image source \"") + rom_name + TEXT("\" can not be processed."));
            return false;
        }

        if (!_load_from(rom, rom_name, image))
        {
            LOG_ERROR(TEXT("The image could not be loaded from \"") + rom_name + TEXT("\"."));
            return false;
        }

        return true;
    }



    bool ImageHandler::_log_bad_format_error(const cl7::string& rom_name)
    {
        LOG_ERROR(TEXT("The format of image \"") + rom_name + TEXT("\" is invalid."));
        return false;
    }

    bool ImageHandler::_log_bad_header_error(const cl7::string& rom_name)
    {
        LOG_ERROR(TEXT("Bad header of image \"") + rom_name + TEXT("\" is damaged."));
        return false;
    }



} // namespace xl7::graphics::images
