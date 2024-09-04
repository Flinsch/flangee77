#include "Matrix2x3.h"



namespace ml7 {



    const Matrix2x3 Matrix2x3::ZERO =       { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
    const Matrix2x3 Matrix2x3::IDENTITY =   { 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Initializes a transformation matrix from the specified scaling vector, a
     * counter-clockwise rotation angle theta (in radians), and a translation vector.
     */
    Matrix2x3 Matrix2x3::compose(const ml7::Vector2& scaling, float theta, const ml7::Vector2& translation)
    {
        const float sx = scaling.x;
        const float sy = scaling.y;
        const float cs = ::cosf( theta );
        const float sn = ::sinf( theta );
        return {
            cs * sx,    -sn * sy,   translation.x,
            sn * sx,    cs * sy,    translation.y,
        };
    }

    /**
     * Swap operation.
     */
    void Matrix2x3::swap(Matrix2x3& rhs)
    {
        Matrix2x3 tmp( rhs );
        rhs = *this;
        *this = tmp;
    }



    // #############################################################################
    // Transformations
    // #############################################################################

    /**
     * Returns a copy of this matrix inverted (if possible).
     */
    Matrix2x3 Matrix2x3::inverted() const
    {
        const float det = determinant();
        if ( det == 0.0f )
            return ZERO;
        const float i = 1.0f / det;
        return {
            _22*i, -_12*i, (_12*_23 - _22*_13)*i,
            -_21*i, _11*i, (_21*_13 - _11*_23)*i,
        };
    }

    /**
     * Tries to extract the scaling vector, the counter-clockwise rotation angle theta
     * (in the range [-pi;+pi]), and the translation vector this matrix is composed of.
     * This only works if the matrix actually consists of translations, rotations and
     * (positive) scalings in the "common" order (no shears, negative scalings, etc.).
     */
    bool Matrix2x3::decompose(ml7::Vector2& scaling, float& theta, ml7::Vector2& translation) const
    {
        const float sx = Vector2( _11, _21 ).length();
        const float sy = Vector2( _12, _22 ).length();
        if ( !sx ) return false;
        if ( !sy ) return false;
        scaling.x = sx;
        scaling.y = sy;
        theta = ::atan2f( -_12 / sy, _11 / sx );
        translation.x = _13;
        translation.y = _23;
        return true;
    }

    /**
     * Returns a copy of the given (column) vector transformed by this matrix.
     * Used to transform position vectors rather than direction vectors.
     */
    Vector2 Matrix2x3::transform_inverted(const Vector2& v) const
    {
        return transform2x2_inverted( v - ml7::Vector2( _13, _23 ) );
    }

    /**
     * Returns a copy of the given (column) vector transformed by the 2x2 part of
     * this matrix.
     * Used to transform direction vectors rather than position vectors.
     */
    Vector2 Matrix2x3::transform2x2_inverted(const Vector2& v) const
    {
        const float det = determinant();
        if ( det == 0.0f )
            return v;
        const float i = 1.0f / det;
        return {
            (_22*v.x + -_12*v.y)*i,
            (-_21*v.x + _11*v.y)*i,
        };
    }



} // namespace ml7
