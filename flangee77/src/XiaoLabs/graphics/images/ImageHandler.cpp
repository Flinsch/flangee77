#include "ImageHandler.h"

#include <CoreLabs/io/file.h>
#include <CoreLabs/logging.h>



namespace xl7::graphics::images {



    /**
     * Loads an image from a file.
     */
    bool ImageHandler::load_from_file(const cl7::u8string& file_path, Image& image)
    {
        cl7::io::file file(file_path, cl7::io::open_mode::read);
        if (!file.is_readable())
        {
            LOG_ERROR(u8"The image file \"" + file_path + u8"\" is not readable. Does it exist?");
            return false;
        }

        return load_from(file, file_path, image);
    }

    /**
     * Loads an image from any rom.
     */
    bool ImageHandler::load_from(cl7::io::irom& rom, const cl7::u8string& rom_name, Image& image)
    {
        if (!rom.is_good())
        {
            LOG_ERROR(u8"The image source \"" + rom_name + u8"\" can not be processed.");
            return false;
        }

        if (!_load_from(rom, rom_name, image))
        {
            LOG_ERROR(u8"The image could not be loaded from \"" + rom_name + u8"\".");
            return false;
        }

        return true;
    }



    bool ImageHandler::_log_bad_format_error(const cl7::u8string& rom_name)
    {
        LOG_ERROR(u8"The format of image \"" + rom_name + u8"\" is invalid.");
        return false;
    }

    bool ImageHandler::_log_bad_header_error(const cl7::u8string& rom_name)
    {
        LOG_ERROR(u8"Bad header of image \"" + rom_name + u8"\" is damaged.");
        return false;
    }



} // namespace xl7::graphics::images
