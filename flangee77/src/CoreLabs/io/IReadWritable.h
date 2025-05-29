#ifndef CL7_IO_IREADWRITABLE_H
#define CL7_IO_IREADWRITABLE_H
#include "./IReadable.h"
#include "./IWritable.h"



namespace cl7::io {



class IReadWritable
    : public IReadable, public IWritable
{

public:
    ~IReadWritable() override = default;

}; // class IReadWritable



} // namespace cl7::io

#endif // CL7_IO_IREADWRITABLE_H
