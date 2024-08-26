#include "Matrix2x2.h"



namespace ml7 {



    const Matrix2x2 Matrix2x2::ZERO =       { 0.0f, 0.0f, 0.0f, 0.0f };
    const Matrix2x2 Matrix2x2::IDENTITY =   {};



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
     * Tries to extract the scaling vector and the counter-clockwise rotation angle
     * theta (in the range [-pi;+pi]) this matrix is composed of.
     * This only works if the matrix actually consists of rotations and (positive)
     * scalings in the "common" order (no shears, negative scalings, etc.).
     */
    void Matrix2x2::decompose(ml7::Vector2& scaling, float& theta) const
    {
        const float sx = Vector2( a, c ).length();
        const float sy = Vector2( b, d ).length();
        scaling.x = sx;
        scaling.y = sy;
        theta = ::atan2f( -b / sy, a / sx );
    }



} // namespace ml7
