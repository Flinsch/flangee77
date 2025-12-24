#include "ImageWriter.h"

#include <CoreLabs/io/File.h>
#include <CoreLabs/logging.h>



namespace xl7::graphics::images {



    /**
     * "Dumps" an image to a file.
     */
    bool ImageWriter::dump_to_file(const Image& image, const cl7::u8string& file_path)
    {
        cl7::io::File file(file_path, cl7::io::OpenMode::Truncate);
        if (!file.is_writable())
        {
            LOG_ERROR(u8"The target file \"" + file_path + u8"\" is not writable.");
            return false;
        }

        return dump_to(image, file, file_path);
    }

    /**
     * "Dumps" an image to any writable object.
     */
    bool ImageWriter::dump_to(const Image& image, cl7::io::IWritable& writable, const cl7::u8string& target_name)
    {
        if (!writable.is_writable())
        {
            LOG_ERROR(u8"The image target \"" + target_name + u8"\" can not be processed.");
            return false;
        }

        if (!_dump_to(image, writable, target_name))
        {
            LOG_ERROR(u8"The image could not be \"dumped\" to \"" + target_name + u8"\".");
            return false;
        }

        return true;
    }



} // namespace xl7::graphics::images
