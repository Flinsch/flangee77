#ifndef CL7_IO_IRESIZABLE_H
#define CL7_IO_IRESIZABLE_H

#include <CoreLabs/root.h>



namespace cl7::io {



class IResizable
{

public:
    virtual ~IResizable() = default;


    /**
     * Returns the current size of the file or whatever, in bytes.
     */
    virtual size_t get_size() const = 0;

    /**
     * (Re)sets the size of the file or whatever, in bytes, and truncates or appends
     * the difference in bytes accordingly. Returns the new/current size.
     */
    virtual size_t set_size(size_t size) = 0;

}; // class IResizable



} // namespace cl7::io

#endif // CL7_IO_IRESIZABLE_H
