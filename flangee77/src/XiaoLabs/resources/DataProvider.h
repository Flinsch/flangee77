#pragma once
#ifndef XL7_RESOURCES_DATAPROVIDER_H
#define XL7_RESOURCES_DATAPROVIDER_H

#include <CoreLabs/byte_span.h>
#include <CoreLabs/byte_vector.h>



namespace xl7::resources {



class DataProvider
{
public:
    virtual ~DataProvider() = default;



    /**
     * Fills the given byte span with the provided data. Any specified offset of
     * this data provider is not taken into account here; this must have been
     * applied beforehand when defining the byte span.
     */
    void fill(cl7::byte_span target) const;

    /**
     * Fills the given byte vector with the provided data. If the specified offset
     * of this data provider is greater than 0, a corresponding number of bytes in
     * the target vector will be skipped. In any case, the byte vector will then
     * have a size of at least the sum of the specified offset and size of this data
     * provider.
     */
    void fill(cl7::byte_vector& target) const;



    /**
     * Returns the target data offset, in bytes.
     */
    virtual size_t get_offset() const { return 0; }

    /**
     * Returns the provided data size, in bytes.
     */
    virtual size_t get_size() const { return 0; }



private:

    /**
     * Fills the given byte span with the provided data. Any specified offset of
     * this data provider is not taken into account here; this must have been
     * applied beforehand when defining the byte span.
     */
    virtual void _fill(cl7::byte_span target) const = 0;

}; // class DataProvider



} // namespace xl7::resources

#endif // XL7_RESOURCES_DATAPROVIDER_H
