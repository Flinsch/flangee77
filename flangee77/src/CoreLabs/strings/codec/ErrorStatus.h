#ifndef CL7_STRINGS_CODEC_ERRORSTATUS_H
#define CL7_STRINGS_CODEC_ERRORSTATUS_H

#include "./EncodingError.h"



namespace cl7::strings::codec {



struct ErrorStatus
{
    /** None/zero if encoding/decoding has been successful; any non-zero value otherwise. */
    EncodingError error = EncodingError::None;
    /** The total number of errors encountered. */
    size_t error_count = 0;



    /** Initializes a status with no errors. */
    ErrorStatus() = default;

    /** Initializes a status with the specified error code and an error counter of 1. */
    ErrorStatus(EncodingError error) : error(error), error_count(1)
    {
        assert(error != EncodingError::None);
    }



    /** Merges this status with the specified error code and increments the error counter by 1. */
    void merge_error(EncodingError error)
    {
        assert(error != EncodingError::None);
        this->error |= error;
        ++error_count;
    }

    /** Combines this status with another and increases the error counter accordingly if necessary. */
    void merge_status(const ErrorStatus& status)
    {
        this->error |= status.error;
        error_count += status.error_count;
    }
};



} // namespace cl7::strings::codec

#endif // CL7_STRINGS_CODEC_ERRORSTATUS_H
