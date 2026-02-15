#include "ImageWriter.h"

#include <CoreLabs/io/File.h>
#include <CoreLabs/io/WritableMemory.h>
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

        cl7::io::WritableMemory writable_memory;

        if (!dump_to(image, writable_memory, file_path))
            return false;

        return file.write(writable_memory.get_data()) == writable_memory.get_data().size();
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



    static bool _log_error(const cl7::u8string& base_message, const cl7::u8string& description)
    {
        if (description.empty())
            LOG_ERROR(base_message + u8".");
        else
            LOG_ERROR(base_message + u8": " + description);
        return false;
    }

    bool ImageWriter::_log_unsupported_format_error(const cl7::u8string& target_name, const cl7::u8string& description)
    {
        return _log_error(u8"The specific (sub)format of image \"" + target_name + u8"\" is not supported", description);
    }

    bool ImageWriter::_log_bad_data_error(const cl7::u8string& target_name, const cl7::u8string& description)
    {
        return _log_error(u8"Bad data of image \"" + target_name + u8"\" is damaged", description);
    }



} // namespace xl7::graphics::images
