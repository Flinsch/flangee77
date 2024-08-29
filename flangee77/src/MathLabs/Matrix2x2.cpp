#include "Matrix2x2.h"



namespace ml7 {



    const Matrix2x2 Matrix2x2::ZERO =       { 0.0f, 0.0f, 0.0f, 0.0f };
    const Matrix2x2 Matrix2x2::IDENTITY =   { 1.0f, 0.0f, 0.0f, 1.0f };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Initializes a transformation matrix from the specified scaling vector and a
     * counter-clockwise rotation angle theta (in radians).
     */
    Matrix2x2 Matrix2x2::compose(const ml7::Vector2& scaling, float theta)
    {
        const float sx = scaling.x;
        const float sy = scaling.y;
        const float cs = ::cosf( theta );
        const float sn = ::sinf( theta );
        return {
            cs * sx,    -sn * sy,
            sn * sx,    cs * sy,
        };
    }

    /**
     * Swap operation.
     */
    void Matrix2x2::swap(Matrix2x2& rhs)
    {
        Matrix2x2 tmp( rhs );
        rhs = *this;
        *this = tmp;
    }



    // #############################################################################
    // Transformations
    // #############################################################################

    /**
     * Returns a copy of this matrix inverted (if possible).
     */
    Matrix2x2 Matrix2x2::inverted() const
    {
        const float det = determinant();
        if ( det == 0.0f )
            return ZERO;
        const float i = 1.0f / det;
        return {
            _22*i, -_12*i,
            -_21*i, _11*i,
        };
    }

    /**
     * Tries to extract the scaling vector and the counter-clockwise rotation angle
     * theta (in the range [-pi;+pi]) this matrix is composed of.
     * This only works if the matrix actually consists of rotations and (positive)
     * scalings in the "common" order (no shears, negative scalings, etc.).
     */
    void Matrix2x2::decompose(ml7::Vector2& scaling, float& theta) const
    {
        const float sx = Vector2( _11, _21 ).length();
        const float sy = Vector2( _12, _22 ).length();
        scaling.x = sx;
        scaling.y = sy;
        theta = ::atan2f( -_12 / sy, _11 / sx );
    }

    /**
     * Returns a copy of the given (column) vector transformed by this matrix
     * inverted (if possible).
     */
    Vector2 Matrix2x2::transform_inverted(const Vector2& v) const
    {
        const float det = determinant();
        if ( det == 0.0f )
            return v;
        const float i = 1.0f / det;
        return {
            _22*i*v.x + -_12*i*v.y,
            -_21*i*v.x + _11*i*v.y,
        };
    }



} // namespace ml7
