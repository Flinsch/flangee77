#include "Base64.h"



namespace cl7 {



    bool Base64::_initialized = false;
    char Base64::_base64_char_table[ 64 ];
    uint8_t Base64::_base64_sextet_lookup[ 128 ];



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Default/explicit constructor.
     */
    Base64::Base64(const Options& options)
        : _options( options )
    {
        if ( _initialized )
            return;

        for ( uint8_t i = 0; i < 128; ++i )
            _base64_sextet_lookup[ i ] = static_cast<uint8_t>( -1 );

        for ( uint8_t i = 0; i < 26; ++i )
        {
            _base64_char_table[ i ] = static_cast<char>( 'A' + i );
            _base64_sextet_lookup[ static_cast<uint8_t>( 'A' + i ) ] = i;
        }
        for ( uint8_t i = 0; i < 26; ++i )
        {
            _base64_char_table[ i + 26 ] = static_cast<char>( 'a' + i );
            _base64_sextet_lookup[ static_cast<uint8_t>( 'a' + i ) ] = i + 26;
        }
        for ( uint8_t i = 0; i < 10; ++i )
        {
            _base64_char_table[ i + 52 ] = static_cast<char>( '0' + i );
            _base64_sextet_lookup[ static_cast<uint8_t>( '0' + i ) ] = i + 52;
        }
        _base64_char_table[ 62 ] = static_cast<char>( '+' );
        _base64_sextet_lookup[ static_cast<uint8_t>( '+' ) ] = 62;
        _base64_char_table[ 63 ] = static_cast<char>( '/' );
        _base64_sextet_lookup[ static_cast<uint8_t>( '/' ) ] = 63;

        _initialized = true;
    }



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Encodes the specified data as Base64.
     */
    cl7::astring Base64::encode(cl7::byte_view data)
    {
        size_t data_size = data.size();

        cl7::astring base64;
        base64.reserve( data_size * 140/100 ); // Base64 encoding causes an overhead of 33-37% relative to the size of the original binary data.

        size_t _24_bit_count = data_size / 3;
        size_t _16_bit_count = (data_size % 3) / 2;
        size_t _8_bit_count = (data_size % 3) % 2;

        const uint8_t* ptr = reinterpret_cast<const uint8_t*>( data.data() );
        size_t line_length = 0;

        while ( _24_bit_count-- )
        {
            base64 += _encode( _char1( ptr[0] ) );
            base64 += _encode( _char2( ptr[0], ptr[1] ) );
            base64 += _encode( _char3( ptr[1], ptr[2] ) );
            base64 += _encode( _char4( ptr[2] ) );
            ptr += 3;

            line_length += 4;

            bool data_left = _24_bit_count || _16_bit_count || _8_bit_count;

            if ( data_left && _options.insert_breaks && (_options.line_length > 0 && line_length >= _options.line_length) )
            {
                base64 += cl7::achar_type('\n');
                line_length = 0;
            }
            /*else if ( data_left && _options.insert_spaces )
            {
                base64 += cl7::achar_type(' ');
                ++line_length;
            }*/
        }

        assert( _16_bit_count <= 1 );
        assert( _8_bit_count <= 1 );
        assert( !_16_bit_count || !_8_bit_count );

        if ( _16_bit_count )
        {
            base64 += _encode( _char1( ptr[0] ) );
            base64 += _encode( _char2( ptr[0], ptr[1] ) );
            base64 += _encode( _char3( ptr[1], 0 ) );
            ptr += 2;

            if ( _options.pad )
                base64.append( 1, cl7::achar_type('=') );
        }
        else if ( _8_bit_count )
        {
            base64 += _encode( _char1( ptr[0] ) );
            base64 += _encode( _char2( ptr[0], 0 ) );
            ptr += 1;

            if ( _options.pad )
                base64.append( 2, cl7::achar_type('=') );
        }

        assert( ptr == reinterpret_cast<const uint8_t*>( data.data() + data_size ) );

        return base64;
    }

    /**
     * Decodes the specified Base64 text. If the text is invalid, an empty buffer is
     * returned.
     */
    cl7::byte_vector Base64::decode(cl7::astring_view base64)
    {
        cl7::astring base64_;
        base64_.reserve( base64.length() / 4 * 3 + 3 );

        for ( size_t i = 0; i < base64.length(); ++i )
        {
            auto chr = base64[ i ];

            if ( chr == cl7::achar_type(' ') ) continue;
            if ( chr == cl7::achar_type('\n') ) continue;
            if ( chr == cl7::achar_type('\r') ) continue;

            base64_ += chr;
        }

        if ( base64_.empty() )
            return {};

        if ( base64_.length() % 4 == 1 )
            return {};
        else if ( base64_.length() % 4 == 2 )
            base64_.append( 2, cl7::achar_type('=') );
        else if ( base64_.length() % 4 == 3 )
            base64_.append( 1, cl7::achar_type('=') );

        assert( base64_.length() >= 4 );
        assert( base64_.length() % 4 == 0 );
        size_t pad = 0;
        if ( base64_[ base64_.length() - 1 ] == cl7::achar_type('=') ) ++pad;
        if ( base64_[ base64_.length() - 2 ] == cl7::achar_type('=') ) ++pad;
        size_t data_size = base64_.length() / 4 * 3 - pad;
        assert( data_size > 0 );

        cl7::byte_vector data;
        data.reserve( data_size );

        size_t _24_bit_count = data_size / 3;
        size_t _16_bit_count = (data_size % 3) / 2;
        size_t _8_bit_count = (data_size % 3) % 2;

        const cl7::achar_type* ptr = base64_.data();

        while ( _24_bit_count-- )
        {
            auto a = _decode( ptr[0] );
            auto b = _decode( ptr[1] );
            auto c = _decode( ptr[2] );
            auto d = _decode( ptr[3] );
            if ( a >= 64 ) return {};
            if ( b >= 64 ) return {};
            if ( c >= 64 ) return {};
            if ( d >= 64 ) return {};
            data.push_back( static_cast<std::byte>( _byte1( a, b ) ) );
            data.push_back( static_cast<std::byte>( _byte2( b, c ) ) );
            data.push_back( static_cast<std::byte>( _byte3( c, d ) ) );
            ptr += 4;
        }

        assert( _16_bit_count <= 1 );
        assert( _8_bit_count <= 1 );
        assert( !_16_bit_count || !_8_bit_count );

        if ( _16_bit_count )
        {
            auto a = _decode( ptr[0] );
            auto b = _decode( ptr[1] );
            auto c = _decode( ptr[2] );
            if ( a >= 64 ) return {};
            if ( b >= 64 ) return {};
            if ( c >= 64 ) return {};
            data.push_back( static_cast<std::byte>( _byte1( a, b ) ) );
            data.push_back( static_cast<std::byte>( _byte2( b, c ) ) );
            ptr += 3;
        }
        else if ( _8_bit_count )
        {
            auto a = _decode( ptr[0] );
            auto b = _decode( ptr[1] );
            if ( a >= 64 ) return {};
            if ( b >= 64 ) return {};
            data.push_back( static_cast<std::byte>( _byte1( a, b ) ) );
            ptr += 2;
        }

        assert( ptr == base64_.data() + base64_.length() - pad );
        assert( data.size() == data_size );

        return data;
    }

    /**
     * Calculates the number of bytes required to store the decoded data of the
     * specified Base64 text. If the text has an invalid length, 0 is returned.
     * However, the function does not fully validate the data.
     */
    size_t Base64::calculate_data_size(cl7::astring_view base64)
    {
        size_t length = 0;

        for ( size_t i = 0; i < base64.length(); ++i )
        {
            auto chr = base64[ i ];

            if ( chr == cl7::achar_type(' ') ) continue;
            if ( chr == cl7::achar_type('\n') ) continue;
            if ( chr == cl7::achar_type('\r') ) continue;

            if ( chr == cl7::achar_type('=') ) continue;

            ++length;
        }

        if ( length == 0 )
            return 0;

        if ( length % 4 == 1 )
            return 0;
        else if ( length % 4 == 2 )
            return length / 4 * 3 + 1;
        else if ( length % 4 == 3 )
            return length / 4 * 3 + 2;

        return length / 4 * 3;
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Extracts the first 6-bit Base64 character sextet from the first data byte.
     */
    uint8_t Base64::_char1(uint8_t a) const
    {
        return (a >> 2) & 0x3f;
    }

    /**
     * Extracts the second 6-bit Base64 character sextet from the first two data bytes.
     */
    uint8_t Base64::_char2(uint8_t a, uint8_t b) const
    {
        return ((a & 0x03) << 4) | ((b >> 4) & 0x0f);
    }

    /**
     * Extracts the third 6-bit Base64 character sextet from the second and third data bytes.
     */
    uint8_t Base64::_char3(uint8_t b, uint8_t c) const
    {
        return ((b & 0x0f) << 2) | ((c >> 6) & 0x3f);
    }

    /**
     * Extracts the fourth 6-bit Base64 character sextet from the third data byte.
     */
    uint8_t Base64::_char4(uint8_t c) const
    {
        return c & 0x3f;
    }

    /**
     * Extracts the first data byte from the first and second 6-bit Base64 character sextets.
     */
    uint8_t Base64::_byte1(uint8_t a, uint8_t b) const
    {
        assert( a <= 0x3f );
        assert( b <= 0x3f );
        return (a << 2) | (b >> 4);
    }

    /**
     * Extracts the second data byte from the second and third 6-bit Base64 character sextets.
     */
    uint8_t Base64::_byte2(uint8_t b, uint8_t c) const
    {
        assert( b <= 0x3f );
        assert( c <= 0x3f );
        return (b << 4) | (c >> 2);
    }

    /**
     * Extracts the third data byte from the given third and fourth 6-bit Base64 character sextets.
     */
    uint8_t Base64::_byte3(uint8_t c, uint8_t d) const
    {
        assert( c <= 0x3f );
        assert( d <= 0x3f );
        return (c << 6) | d;
    }

    /**
     * Returns the Base64 character of the given 6-bit character sextet.
     */
    cl7::achar_type Base64::_encode(uint8_t sextet) const
    {
        if ( sextet == 62 ) return _options.ch62;
        if ( sextet == 63 ) return _options.ch63;

        assert( sextet < 64 );
        return _base64_char_table[ sextet ];
    }

    /**
     * Checks the given character whether it is a valid Base64 character.
     * If it is valid, its 6-bit character sextet will be returned.
     * If it is invalid, an invalid value above 63 will be returned.
     */
    uint8_t Base64::_decode(cl7::achar_type chr) const
    {
        if ( chr == _options.ch62 ) return 62;
        if ( chr == _options.ch63 ) return 63;

        size_t lookup = static_cast<size_t>( chr );
        if ( lookup >= 128 )
            return static_cast<uint8_t>( -1 );

        uint8_t sextet = _base64_sextet_lookup[ lookup ];
        if ( _base64_char_table[ sextet ] != chr )
            return static_cast<uint8_t>( -1 );

        return sextet;
    }



} // namespace cl7
