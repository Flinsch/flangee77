#include "Signature.h"



namespace tl7 {



    bool Signature::operator == (const Signature& rhs) const
    {
        assert( file_path == rhs.file_path );
        return line_number == rhs.line_number && iteration_number == rhs.iteration_number;
    }



    size_t Signature::hash::operator () (const Signature& sig) const
    {
        constexpr auto s = sizeof(size_t) / 2 * 8;
        constexpr size_t m = (size_t(1) << s) - 1;
        const size_t hi = static_cast<size_t>( sig.line_number );
        const size_t lo = static_cast<size_t>( sig.iteration_number < 0 ? m : sig.iteration_number );
        assert( hi < m );
        assert( lo <= m );
        return (hi << s) | lo;
    }

    bool Signature::equal_to::operator () (const Signature& sig1, const Signature& sig2) const
    {
        return sig1 == sig2;
    }



} // namespace tl7
