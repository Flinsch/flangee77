#include "DefaultDataProvider.h"



namespace xl7 {
namespace resources {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Default constructor.
     */
    DefaultDataProvider::DefaultDataProvider()
        : _data()
        , _offset( 0 )
    {
    }

    /**
     * Explicit constructor.
     */
    DefaultDataProvider::DefaultDataProvider(cl7::byte_view data, size_t offset)
        : _data( data )
        , _offset( offset )
    {
    }



    // #############################################################################
    // Implementations
    // #############################################################################

    /**
     * Fills the given byte span with the provided data. Any specified offset of
     * this data provider is not taken into account here; this must have been
     * applied beforehand when defining the byte span.
     */
    void DefaultDataProvider::_fill(cl7::byte_span target) const
    {
        assert( target.size() >= _data.size() );
        std::copy( _data.begin(), _data.end(), target.begin() );
    }



} // namespace resources
} // namespace xl7
