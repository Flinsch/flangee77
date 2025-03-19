#ifndef CL7_STRINGS_RESULTBASE_H
#define CL7_STRINGS_RESULTBASE_H

#include "./EncodingError.h"



namespace cl7::strings {



struct BaseResult
{
    /** None/zero if encoding/decoding has been successful; any non-zero value otherwise. */
    EncodingError error = EncodingError::None;
    /** The total number of errors encountered. */
    size_t error_count = 0;



    /** Initializes a result with no errors. */
    BaseResult() = default;

    /** Initializes a result with the specified error code and an error counter of 1. */
    BaseResult(EncodingError error) : error(error), error_count(1) {}



    /** Merges this result with the specified error code and increments the error counter by 1. */
    void merge_error(EncodingError error)
    {
        this->error |= error;
        ++error_count;
    }
};



} // namespace cl7::strings

#endif // CL7_STRINGS_RESULTBASE_H
