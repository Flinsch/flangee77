#ifndef XL7_RESOURCES_RESOURCEDATAMIXIN_H
#define XL7_RESOURCES_RESOURCEDATAMIXIN_H
#include "./Resource.h"



namespace xl7::resources {



template <class TDerived>
class ResourceDataMixin
{

public:
    /** Returns the local copy of the resource data. */
    cl7::byte_view get_data() const { return _data; }



protected:
    explicit ResourceDataMixin(size_t data_size)
        : _data(data_size)
    {
    }



    /** Provides mutable access to the local copy of the resource data. */
    cl7::byte_span _access_data() { return _data; }



private:
    /** The local copy of the resource data. */
    cl7::byte_vector _data;

};



} // namespace xl7::resources

#endif // XL7_RESOURCES_RESOURCEDATAMIXIN_H
