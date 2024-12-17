#include "Matrix4x4.h"



namespace ml7 {



    const Matrix4x4 Matrix4x4::ZERO =       {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    const Matrix4x4 Matrix4x4::IDENTITY =   {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Swap operation.
     */
    void Matrix4x4::swap(Matrix4x4& other) noexcept
    {
        Matrix4x4 tmp(other);
        other = *this;
        *this = tmp;
    }



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Returns the determinant of the matrix.
     */
    float Matrix4x4::determinant() const
    {
        if (is_affine())
        {
            return
                + _11*_22*_33 + _12*_23*_31 + _13*_21*_32
                - _31*_22*_13 - _32*_23*_11 - _33*_21*_12;
        }

        // For the following calculations see:
        // http://www.geometrictools.com/Documentation/LaplaceExpansionTheorem.pdf
        const float s1 = _11*_22 - _12*_21;
        const float c1 = _33*_44 - _34*_43;
        const float s2 = _11*_23 - _13*_21;
        const float c2 = _32*_44 - _34*_42;
        const float s3 = _11*_24 - _14*_21;
        const float c3 = _32*_43 - _33*_42;
        const float s4 = _12*_23 - _13*_22;
        const float c4 = _31*_44 - _34*_41;
        const float s5 = _12*_24 - _14*_22;
        const float c5 = _31*_43 - _33*_41;
        const float s6 = _13*_24 - _14*_23;
        const float c6 = _31*_42 - _32*_41;

        return s1*c1 - s2*c2 + s3*c3 + s4*c4 - s5*c5 + s6*c6;
    }



    // #############################################################################
    // Transformations
    // #############################################################################

    /**
     * Returns a copy of this matrix inverted (if possible).
     */
    Matrix4x4 Matrix4x4::inverted() const
    {
        if (is_affine())
        {
            return Matrix4x4(to_matrix3x4().inverted());
        }

        // For the following calculations see:
        // http://www.geometrictools.com/Documentation/LaplaceExpansionTheorem.pdf
        const float s1 = _11*_22 - _12*_21;
        const float c1 = _33*_44 - _34*_43;
        const float s2 = _11*_23 - _13*_21;
        const float c2 = _32*_44 - _34*_42;
        const float s3 = _11*_24 - _14*_21;
        const float c3 = _32*_43 - _33*_42;
        const float s4 = _12*_23 - _13*_22;
        const float c4 = _31*_44 - _34*_41;
        const float s5 = _12*_24 - _14*_22;
        const float c5 = _31*_43 - _33*_41;
        const float s6 = _13*_24 - _14*_23;
        const float c6 = _31*_42 - _32*_41;

        const float det = s1*c1 - s2*c2 + s3*c3 + s4*c4 - s5*c5 + s6*c6;
        if (det == 0.0f)
            return ZERO;
        const float i = 1.0f / det;
        return Matrix4x4{
            +_22*c1 - _23*c2 + _24*c3,   -_12*c1 + _13*c2 - _14*c3,   +_42*s6 - _43*s5 + _44*s4,   -_32*s6 + _33*s5 - _34*s4,
            -_21*c1 + _23*c4 - _24*c5,   +_11*c1 - _13*c4 + _14*c5,   -_41*s6 + _43*s3 - _44*s2,   +_31*s6 - _33*s3 + _34*s2,
            +_21*c2 - _22*c4 + _24*c6,   -_11*c2 + _12*c4 - _14*c6,   +_41*s5 - _42*s3 + _44*s1,   -_31*s5 + _32*s3 - _34*s1,
            -_21*c3 + _22*c5 - _23*c6,   +_11*c3 - _12*c5 + _13*c6,   -_41*s4 + _42*s2 - _43*s1,   +_31*s4 - _32*s2 + _33*s1,
        } * i;
    }

    /**
     * Tries to extract the rotation axis and the rotation angle (in the range
     * [0;pi]) the 3x3 part of this matrix is composed of.
     * This only works if the 3x3 part actually consists of rotations only (no
     * scalings, shears, etc.).
     */
    bool Matrix4x4::to_axis_angle(ml7::Vector3& axis, float& angle) const
    {
        return to_matrix3x3().to_axis_angle(axis, angle);
    }

    /**
     * Tries to extract the scaling vector, the rotation axis, the rotation angle
     * (in the range [0;pi]), and the translation vector this matrix is composed of.
     * This only works if the matrix actually consists of translations, rotations and
     * (positive) scalings in the "common" order (no projections, shears, negative
     * scalings, etc.).
     */
    bool Matrix4x4::decompose(ml7::Vector3& scaling, ml7::Vector3& axis, float& angle, ml7::Vector3& translation) const
    {
        if (!is_affine())
            return false;

        translation.x = _14;
        translation.y = _24;
        translation.z = _34;
        return to_matrix3x3().decompose(scaling, axis, angle);
    }

    /**
     * Assumes that this matrix is a view transformation matrix for a left-handed
     * coordinate system and tries to extract the camera position, the camera "look"
     * direction, and the "up" direction.
     */
    bool Matrix4x4::is_look_lh(ml7::Vector3& position, ml7::Vector3& look, ml7::Vector3& up) const
    {
        if (!is_affine())
            return false;

        // Since we assume that the 3x3 part is a rotation matrix,
        // we can simply transpose it (instead of "costly" inverting it).
        position = {
            -(_11*_14 + _21*_24 + _31*_34),
            -(_12*_14 + _22*_24 + _32*_34),
            -(_13*_14 + _23*_24 + _33*_34),
        };
        look = {_31, _32, _33};
        up = {_21, _22, _23};
        return look.lensqr() != 0.0f && up.lensqr() != 0.0f; // Just some bare minimum check.
    }

    /**
     * Assumes that this matrix is a view transformation matrix for a right-handed
     * coordinate system and tries to extract the camera position, the camera "look"
     * direction, and the "up" direction.
     */
    bool Matrix4x4::is_look_rh(ml7::Vector3& position, ml7::Vector3& look, ml7::Vector3& up) const
    {
        if (!is_affine())
            return false;

        // Since we assume that the 3x3 part is a rotation matrix,
        // we can simply transpose it (instead of "costly" inverting it).
        position = {
            -(_11*_14 + _21*_24 + _31*_34),
            -(_12*_14 + _22*_24 + _32*_34),
            -(_13*_14 + _23*_24 + _33*_34),
        };
        look = {-_31, -_32, -_33};
        up = {_21, _22, _23};
        return look.lensqr() != 0.0f && up.lensqr() != 0.0f; // Just some bare minimum check.
    }

    /**
     * Assumes that this matrix is an orthogonal projection matrix for a left-handed
     * coordinate system and tries to extract the width and the height of the view
     * volume as well as the distances to the near and far clipping planes.
     */
    bool Matrix4x4::is_orthographic_lh(float& width, float& height, float& znear, float& zfar) const
    {
        if (_41 != 0.0f || _42 != 0.0f || _43 != 0.0f || _44 != 1.0f)
            return false;

        if (_12 != 0.0f || _13 != 0.0f || _21 != 0.0f || _23 != 0.0f || _31 != 0.0f || _32 != 0.0f)
            return false;
        if (_14 != 0.0f || _24 != 0.0f)
            return false;

        if (_11 == 0.0f || _22 == 0.0f || _33 == 0.0f || _34 == 0.0f)
            return false;

        width = 2.0f / _11;
        height = 2.0f / _22;
        znear = -_34 / _33;
        zfar = (1.0f - _34) / _33;
        return true;
    }

    /**
     * Assumes that this matrix is an orthogonal projection matrix for a right-handed
     * coordinate system and tries to extract the width and the height of the view
     * volume as well as the distances to the near and far clipping planes.
     */
    bool Matrix4x4::is_orthographic_rh(float& width, float& height, float& znear, float& zfar) const
    {
        if (_41 != 0.0f || _42 != 0.0f || _43 != 0.0f || _44 != 1.0f)
            return false;

        if (_12 != 0.0f || _13 != 0.0f || _21 != 0.0f || _23 != 0.0f || _31 != 0.0f || _32 != 0.0f)
            return false;
        if (_14 != 0.0f || _24 != 0.0f)
            return false;

        if (_11 == 0.0f || _22 == 0.0f || _33 == 0.0f || _34 == 0.0f)
            return false;

        width = 2.0f / _11;
        height = 2.0f / _22;
        znear = _34 / _33;
        zfar = (_34 - 1.0f) / _33;
        return true;
    }

    /**
     * Assumes that this matrix in an orthogonal projection matrix (regardless of
     * whether it is left- or right-handed) and tries to extract the width and the
     * height of the view volume as well as the distances to the near and far
     * clipping planes.
     */
    bool Matrix4x4::is_orthographic(float& width, float& height, float& znear, float& zfar) const
    {
        if (_33 > 0.0f) return is_orthographic_lh(width, height, znear, zfar);
        if (_33 < 0.0f) return is_orthographic_rh(width, height, znear, zfar);
        return false;
    }

    /**
     * Assumes that this matrix is a perspective projection matrix for a left-handed
     * coordinate system and tries to extract the vertical field-of-view angle (in
     * radians), the aspect ratio of the frustum's width to its height, and the
     * distances to the near and far clipping planes.
     */
    bool Matrix4x4::is_perspective_lh(float& fov, float& aspect, float& znear, float& zfar) const
    {
        if (_41 != 0.0f || _42 != 0.0f || _43 != 1.0f || _44 != 0.0f)
            return false;

        if (_12 != 0.0f || _13 != 0.0f || _21 != 0.0f || _23 != 0.0f || _31 != 0.0f || _32 != 0.0f)
            return false;
        if (_14 != 0.0f || _24 != 0.0f)
            return false;

        if (_11 == 0.0f || _22 == 0.0f || _33 == 0.0f || _34 == 0.0f)
            return false;

        fov = 2.0f * ::atanf(1.0f / _22);
        aspect = _22 / _11;
        znear = -_34 / _33;
        zfar = _34 / (1.0f - _33);
        return true;
    }

    /**
     * Assumes that this matrix is a perspective projection matrix for a right-handed
     * coordinate system and tries to extract the vertical field-of-view angle (in
     * radians), the aspect ratio of the frustum's width to its height, and the
     * distances to the near and far clipping planes.
     */
    bool Matrix4x4::is_perspective_rh(float& fov, float& aspect, float& znear, float& zfar) const
    {
        if (_41 != 0.0f || _42 != 0.0f || _43 != -1.0f || _44 != 0.0f)
            return false;

        if (_12 != 0.0f || _13 != 0.0f || _21 != 0.0f || _23 != 0.0f || _31 != 0.0f || _32 != 0.0f)
            return false;
        if (_14 != 0.0f || _24 != 0.0f)
            return false;

        if (_11 == 0.0f || _22 == 0.0f || _33 == 0.0f || _34 == 0.0f)
            return false;

        fov = 2.0f * ::atanf(1.0f / _22);
        aspect = _22 / _11;
        znear = _34 / _33;
        zfar = _34 / (1.0f + _33);
        return true;
    }

    /**
     * Assumes that this matrix is a perspective projection matrix (regardless of
     * whether it is left- or right-handed) and tries to extract the vertical
     * field-of-view angle (in radians), the aspect ratio of the frustum's width to
     * its height, and the distances to the near and far clipping planes.
     */
    bool Matrix4x4::is_perspective(float& fov, float& aspect, float& znear, float& zfar) const
    {
        if (_33 > 0.0f) return is_perspective_lh(fov, aspect, znear, zfar);
        if (_33 < 0.0f) return is_perspective_rh(fov, aspect, znear, zfar);
        return false;
    }

    /**
     * Assumes that this matrix is a projection matrix (whether orthogonal or
     * perspective, left- or right-handed) and tries to extract the distances to the
     * near and far clipping planes.
     */
    bool Matrix4x4::is_projection(float& znear, float& zfar) const
    {
        float foo;
        float bar;
        if (_43 == 0.0f && _44 == 1.0f) return is_orthographic(foo, bar, znear, zfar);
        if (_43 != 0.0f && _44 == 0.0f) return is_perspective(foo, bar, znear, zfar);
        return false;
    }

    /**
     * Returns a copy of the given (column) vector transformed by this matrix
     * inverted (if possible).
     * Used to transform position vectors rather than direction vectors.
     */
    Vector4 Matrix4x4::transform_inverted(const Vector4& v) const
    {
        return inverted().transform(v);
    }

    /**
     * Returns a copy of the given (column) vector transformed by this matrix
     * inverted (if possible).
     * Used to transform position vectors rather than direction vectors.
     */
    Vector3 Matrix4x4::transform_inverted(const Vector3& v) const
    {
        return inverted().transform(v);
    }

    /**
     * Returns a copy of the given (column) vector transformed by the 3x3 part of
     * this matrix inverted (if possible).
     * Used to transform direction vectors rather than position vectors.
     */
    Vector3 Matrix4x4::transform3x3_inverted(const Vector3& v) const
    {
        return to_matrix3x3().transform_inverted(v);
    }



    // #############################################################################
    // Static Functions
    // #############################################################################

    /**
     * Initializes a rotation matrix representing a rotation by a certain angle (in
     * radians) around an axis specified as a unit vector.
     */
    Matrix4x4 Matrix4x4::rotation_normalized(const ml7::Vector3& unit_axis, float angle)
    {
        return Matrix4x4(Matrix3x3::rotation_normalized(unit_axis, angle));
    }

    /**
     * Initializes a transformation matrix from the specified scaling vector, a
     * rotation axis, a rotation angle (in radians), and a translation vector.
     */
    Matrix4x4 Matrix4x4::compose(const ml7::Vector3& scaling, const ml7::Vector3& axis, float angle, const ml7::Vector3& translation)
    {
        return Matrix4x4(Matrix3x4::compose(scaling, axis, angle, translation));
    }

    /**
     * Initializes a view transformation matrix for a left-handed coordinate system
     * using a camera position, a focal point, and an "up" direction.
     */
    Matrix4x4 Matrix4x4::look_at_lh(const Vector3& position, const Vector3& focus, const Vector3& up)
    {
        // https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixlookatlh
        return look_to_lh(position, focus - position, up);
    }

    /**
     * Initializes a view transformation matrix for a right-handed coordinate system
     * using a camera position, a focal point, and an "up" direction.
     */
    Matrix4x4 Matrix4x4::look_at_rh(const Vector3& position, const Vector3& focus, const Vector3& up)
    {
        // https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixlookatrh
        return look_to_rh(position, focus - position, up);
    }

    /**
     * Initializes a view transformation matrix for a left-handed coordinate system
     * using a camera position, a camera "look" direction, and an "up" direction.
     */
    Matrix4x4 Matrix4x4::look_to_lh(const Vector3& position, const Vector3& look, const Vector3& up)
    {
        // https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixlookatlh
        const Vector3 z(look.normalized());
        const Vector3 x(up.cross(z).normalized());
        const Vector3 y(z.cross(x));
        return {
            x.x,  x.y,  x.z,  -x.dot(position),
            y.x,  y.y,  y.z,  -y.dot(position),
            z.x,  z.y,  z.z,  -z.dot(position),
            0.0f, 0.0f, 0.0f, 1.0f,
        };
    }

    /**
     * Initializes a view transformation matrix for a right-handed coordinate system
     * using a camera position, a camera "look" direction, and an "up" direction.
     */
    Matrix4x4 Matrix4x4::look_to_rh(const Vector3& position, const Vector3& look, const Vector3& up)
    {
        // https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixlookatrh
        return look_to_lh(position, -look, up);
    }

    /**
     * Initializes an orthogonal projection matrix for a left-handed coordinate
     * system using a width and a height of the view volume as well as distances to
     * the near and far clipping planes.
     */
    Matrix4x4 Matrix4x4::orthographic_lh(float width, float height, float znear, float zfar)
    {
        // https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixortholh
        const float rd = 1.0f / (zfar - znear);
        const float sy = 2.0f / height;
        const float sx = 2.0f / width;
        return {
            sx,    0.0f,  0.0f,  0.0f,
            0.0f,  sy,    0.0f,  0.0f,
            0.0f,  0.0f,  rd,    -znear * rd,
            0.0f,  0.0f,  0.0f,  1.0f,
        };
    }

    /**
     * Initializes an orthogonal projection matrix for a right-handed coordinate
     * system using a width and a height of the view volume as well as distances to
     * the near and far clipping planes.
     */
    Matrix4x4 Matrix4x4::orthographic_rh(float width, float height, float znear, float zfar)
    {
        // https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixorthorh
        return orthographic_lh(width, height, -znear, -zfar);
    }

    /**
     * Initializes a perspective projection matrix for a left-handed coordinate
     * system using a vertical field-of-view angle (in radians), an aspect ratio of
     * the frustum's width to its height, and distances to the near and far clipping
     * planes.
     */
    Matrix4x4 Matrix4x4::perspective_lh(float fov, float aspect, float znear, float zfar)
    {
        // https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixperspectivefovlh
        const float rd = zfar / (zfar - znear);
        const float sy = 1.0f / ::tanf(fov * 0.5f); // cot = 1/tan
        const float sx = sy / aspect;
        return {
            sx,    0.0f,  0.0f,  0.0f,
            0.0f,  sy,    0.0f,  0.0f,
            0.0f,  0.0f,  rd,    -znear * rd,
            0.0f,  0.0f,  1.0f,  0.0f,
        };
    }

    /**
     * Initializes a perspective projection matrix for a right-handed coordinate
     * system using a vertical field-of-view angle (in radians), an aspect ratio of
     * the frustum's width to its height, and distances to the near and far clipping
     * planes.
     */
    Matrix4x4 Matrix4x4::perspective_rh(float fov, float aspect, float znear, float zfar)
    {
        // https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixperspectivefovrh
        const float rd = zfar / (znear - zfar);
        const float sy = 1.0f / ::tanf(fov * 0.5f); // cot = 1/tan
        const float sx = sy / aspect;
        return {
            sx,    0.0f,  0.0f,  0.0f,
            0.0f,  sy,    0.0f,  0.0f,
            0.0f,  0.0f,  rd,    znear * rd,
            0.0f,  0.0f,  -1.0f, 0.0f,
        };
    }

    /**
     * Initializes a color transformation matrix using a luminance vector that
     * specifies the weighted sum of the r, g, and b color components, allowing
     * to customize how each component contributes to the (grayscale) result.
     */
    Matrix4x4 Matrix4x4::luminance(const ml7::Vector3& l)
    {
        // http://www.graficaobscura.com/matrix/index.html
        return {
            l.x,  l.y,  l.z,  0.0f,
            l.x,  l.y,  l.z,  0.0f,
            l.x,  l.y,  l.z,  0.0f,
            0.0f, 0.0f, 0.0f, 1.0f,
        };
    }

    /**
     * Initializes a color transformation matrix using a saturation vector that
     * specifies the respective saturation for the r, g, and b color components.
     * The grayscale weights for the r, g, and b color components are given by the
     * luminance vector l = (0.3086, 0.6094, 0.0820) to maintain the brightness.
     * These values are better for linear RGB colors. The NTSC weights of 0.299,
     * 0.587, and 0.114 are only applicable to RGB colors in a gamma 2.2 color space.
     * Interesting: This can also be used to complement the color by specifying a
     * saturation of -1.
     * Note: When s is set to 0, the matrix becomes a "convert to luminance" matrix
     * transforming the color to grayscale according to the luminance vector.
     * When s is set to 1, the matrix becomes the identity matrix leaving the color
     * unchanged.
     */
    Matrix4x4 Matrix4x4::saturation(const ml7::Vector3& s)
    {
        return saturation(s, ml7::Vector3(0.3086f, 0.6094f, 0.0820f));
    }

    /**
     * Initializes a color transformation matrix using a saturation vector that
     * specifies the respective saturation for the r, g, and b color components.
     * The grayscale weights for the r, g, and b color components are given by the
     * specified luminance vector l to maintain the perceived brightness.
     * Interesting: This can also be used to complement the color by specifying a
     * saturation of -1.
     * Note: When s is set to 0, the matrix becomes a "convert to luminance" matrix
     * transforming the color to grayscale according to the luminance vector.
     * When s is set to 1, the matrix becomes the identity matrix leaving the color
     * unchanged.
     */
    Matrix4x4 Matrix4x4::saturation(const ml7::Vector3& s, const ml7::Vector3& l)
    {
        // http://www.graficaobscura.com/matrix/index.html
        return {
            (1.0f-s.x)*l.x+s.x, (1.0f-s.x)*l.y,     (1.0f-s.x)*l.z,     0.0f,
            (1.0f-s.y)*l.x,     (1.0f-s.y)*l.y+s.y, (1.0f-s.y)*l.z,     0.0f,
            (1.0f-s.z)*l.x,     (1.0f-s.z)*l.y,     (1.0f-s.z)*l.z+s.z, 0.0f,
            0.0f,               0.0f,               0.0f,               1.0f,
        };
    }

    /**
     * Initializes a color transformation matrix using a contrast vector that
     * specifies the respective contrast for the r, g, and b color components.
     * Interesting: This can also be used to alter the color balance.
     * Note: When c is set to 0, the resulting "color" is a constant gray with all
     * three color components set to 0.5. When c is set to 1, the matrix becomes the
     * identity matrix leaving the color unchanged.
     */
    Matrix4x4 Matrix4x4::contrast(const ml7::Vector3& c)
    {
        // http://www.graficaobscura.com/matrix/index.html
        // https://docs.rainmeter.net/tips/colormatrix-guide
        return {
            c.x,   0.0f,  0.0f,  (1.0f-c.x)*0.5f,
            0.0f,  c.y,   0.0f,  (1.0f-c.y)*0.5f,
            0.0f,  0.0f,  c.z,   (1.0f-c.z)*0.5f,
            0.0f,  0.0f,  0.0f,  1.0f,
        };
    }

    /**
     * Initializes a color transformation matrix using a rotation angle (in radians)
     * that specifies how much to rotate the r, g, and b color components.
     * The grayscale weights for the r, g, and b color components are given by the
     * luminance vector l = (0.3086, 0.6094, 0.0820) to maintain the brightness.
     * These values are better for linear RGB colors. The NTSC weights of 0.299,
     * 0.587, and 0.114 are only applicable to RGB colors in a gamma 2.2 color space.
     */
    Matrix4x4 Matrix4x4::hue(float r)
    {
        return hue(r, ml7::Vector3(0.3086f, 0.6094f, 0.0820f));
    }

    /**
     * Initializes a color transformation matrix using a rotation angle (in radians)
     * that specifies how much to rotate the r, g, and b color components.
     * The grayscale weights for the r, g, and b color components are given by the
     * specified luminance vector l to maintain the perceived brightness.
     */
    Matrix4x4 Matrix4x4::hue(float r, const ml7::Vector3& l)
    {
        // http://www.graficaobscura.com/matrix/index.html
        // http://beesbuzz.biz/code/hsv_color_transforms.php
        // http://en.wikipedia.org/wiki/YIQ
        const ml7::Matrix4x4 rgb2yiq{
            l.x,        l.y,         l.z,         0.0f,
            0.595716f,  -0.274453f,  -0.321263f,  0.0f,
            0.211456f,  -0.522591f,  0.311135f,   0.0f,
            0.0f,       0.0f,        0.0f,        1.0f,
        };
        const ml7::Matrix4x4 yiq2rgb = rgb2yiq.inverted();

        const ml7::Matrix4x4 hue_rot = ml7::Matrix4x4::rotx(r);

        return yiq2rgb * hue_rot * rgb2yiq;
    }



} // namespace ml7
