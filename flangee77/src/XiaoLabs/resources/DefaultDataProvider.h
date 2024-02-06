#pragma once
#ifndef XL7_RESOURCES_DEFAULTDATAPROVIDER_H
#define XL7_RESOURCES_DEFAULTDATAPROVIDER_H
#include "./DataProvider.h"

#include <CoreLabs/byte_view.h>



namespace xl7 {
namespace resources {



class DefaultDataProvider
    : public DataProvider
{

    // #############################################################################
    // Construction / Destruction
    // #############################################################################
public:
    /**
     * Explicit constructor.
     */
    DefaultDataProvider(cl7::byte_view data, size_t offset = 0);

    /**
     * Destructor.
     */
    virtual ~DefaultDataProvider() = default;



    // #############################################################################
    // Attributes
    // #############################################################################
private:
    const cl7::byte_view _data;
    const size_t _offset;



    // #############################################################################
    // Implementations
    // #############################################################################
public:
    /**
     * Returns the target data offset, in bytes.
     */
    virtual size_t get_offset() const override { return _offset; }

    /**
     * Returns the provided data size, in bytes.
     */
    virtual size_t get_size() const override { return _data.size(); }

private:
    /**
     * Fills the given byte span with the provided data. Any specified offset of
     * this data provider is not taken into account here; this must have been
     * applied beforehand when defining the byte span.
     */
    virtual void _fill(cl7::byte_span target) const override;

}; // class DefaultDataProvider



} // namespace resources
} // namespace xl7

#endif // XL7_RESOURCES_DEFAULTDATAPROVIDER_H
