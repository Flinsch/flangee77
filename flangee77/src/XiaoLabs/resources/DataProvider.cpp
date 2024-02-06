#include "DataProvider.h"



namespace xl7 {
namespace resources {



    // #############################################################################
    // Prototypes
    // #############################################################################

    /**
     * Fills the given byte span with the provided data. Any specified offset of
     * this data provider is not taken into account here; this must have been
     * applied beforehand when defining the byte span.
     */
    void DataProvider::fill(cl7::byte_span target) const
    {
        const size_t size = get_size();
        if ( size == 0 )
            return; // Nothing to do here.

        assert( size <= target.size() );

        return _fill( target );
    }

    /**
     * Fills the given byte vector with the provided data. If the specified offset
     * of this data provider is greater than 0, a corresponding number of bytes in
     * the target vector will be skipped. In any case, the byte vector will then
     * have a size of at least the sum of the specified offset and size of this data
     * provider.
     */
    void DataProvider::fill(cl7::byte_vector& target) const
    {
        const size_t size = get_size();
        if ( size == 0 )
            return; // Nothing to do here.

        const size_t offset = get_offset();

        const size_t min_total_size = offset + size;
        if ( target.size() < min_total_size )
            target.resize( min_total_size );

        return _fill( cl7::byte_span( target.begin() + offset, size ) );
    }



} // namespace resources
} // namespace xl7
