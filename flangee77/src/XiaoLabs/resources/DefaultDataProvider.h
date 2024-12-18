#pragma once
#ifndef XL7_RESOURCES_DEFAULTDATAPROVIDER_H
#define XL7_RESOURCES_DEFAULTDATAPROVIDER_H
#include "./DataProvider.h"

#include <CoreLabs/byte_view.h>



namespace xl7::resources {



class DefaultDataProvider
    : public DataProvider
{

public:
    DefaultDataProvider() = default;

    DefaultDataProvider(cl7::byte_view data, size_t offset = 0);

    ~DefaultDataProvider() override = default;



    /**
     * Returns the target data offset, in bytes.
     */
    size_t get_offset() const override { return _offset; }

    /**
     * Returns the provided data size, in bytes.
     */
    size_t get_size() const override { return _data.size(); }



private:

    /**
     * Fills the given byte span with the provided data. Any specified offset of
     * this data provider is not taken into account here; this must have been
     * applied beforehand when defining the byte span.
     */
    void _fill(cl7::byte_span target) const override;



    cl7::byte_view _data;
    size_t _offset = 0;

}; // class DefaultDataProvider



} // namespace xl7::resources

#endif // XL7_RESOURCES_DEFAULTDATAPROVIDER_H
