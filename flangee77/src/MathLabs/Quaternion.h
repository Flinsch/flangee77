#ifndef ML7_QUATERNION_H
#define ML7_QUATERNION_H

#include "./Matrix3x3.h"
#include "./Vector3.h"



namespace ml7 {



template <std::floating_point T>
struct Quaternion
{

    using scalar_type = T;

    static const Quaternion ZERO;
    static const Quaternion IDENTITY;



    // #############################################################################
    // Attributes
    // #############################################################################

    union
    {
        struct
        {
            // NOLINTBEGIN(*-use-default-member-init)
            /** The quaternion's first imaginary component. */
            T x;
            /** The quaternion's second imaginary component. */
            T y;
            /** The quaternion's third imaginary component. */
            T z;
            /** The quaternion's scalar/real part. */
            T w;
            // NOLINTEND(*-use-default-member-init)
        }; // struct

        /** Array of all four components. */
        T data[4];
    }; // union



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Default constructor. Initializes the quaternion with v = (0, 0, 0) and s = 1.
     */
    constexpr Quaternion() noexcept
        : x(T{0})
        , y(T{0})
        , z(T{0})
        , w(T{1})
    {
    }

    /**
     * Explicit constructor with parameters for the vector/imaginary part
     * v = (x, y, z) and the scalar/real part s = w.
     */
    constexpr Quaternion(T x, T y, T z, T w) noexcept
        : x(x)
        , y(y)
        , z(z)
        , w(w)
    {
    }

    /**
     * Explicit constructor with parameters for the vector/imaginary part v and the
     * scalar/real part s.
     */
    constexpr Quaternion(const Vector3<T>& v, T s) noexcept
        : x(v.x)
        , y(v.y)
        , z(v.z)
        , w(s)
    {
    }

    /**
     * Explicit constructor that initializes a quaternion from the specified
     * rotation matrix.
     */
    constexpr explicit Quaternion(const Matrix3x3<T>& m)
    {
        // https://www.ljll.fr/~frey/papers/scientific%20visualisation/Shoemake%20K.,%20Quaternions.pdf
        const T t = m._11 + m._22 + m._33;
        if (t >= T{0})
        {
            const T r = std::sqrt(t + T{1});
            const T _05i = T{0.5} / r;
            x = (m._32 - m._23) * _05i;
            y = (m._13 - m._31) * _05i;
            z = (m._21 - m._12) * _05i;
            w = T{0.5} * r;
        }
        else // => t < T{0}
        {
            unsigned i = 0;
            if (m._22 > m._11) i = 1;
            if (m._33 > m[i][i]) i = 2;
            unsigned j = (i + 1) % 3;
            unsigned k = (i + 2) % 3;

            const T r = std::sqrt(m[i][i] - m[j][j] - m[k][k] + T{1});
            const T _05i = T{0.5} / r;
            data[i] = T{0.5} * r;
            data[j] = (m[i][j] + m[j][i]) * _05i;
            data[k] = (m[k][i] + m[i][k]) * _05i;
            w       = (m[k][j] - m[j][k]) * _05i;
        }
    }

    /**
     * Swap operation.
     */
    void swap(Quaternion& other) noexcept
    {
        std::swap(x, other.x);
        std::swap(y, other.y);
        std::swap(z, other.z);
        std::swap(w, other.w);
    }



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Tells whether this quaternion is invertible (i.e., whether it is non-zero).
     */
    bool is_invertible() const { return (x*x + y*y + z*z + w*w) != T{0}; }

    /**
     * Returns the magnitude of this quaternion.
     */
    T norm() const { return std::sqrt(x*x + y*y + z*z + w*w); }



    // #############################################################################
    // Transformations
    // #############################################################################

    /**
     * Returns a copy of this quaternion normalized (if possible).
     */
    Quaternion normalized() const
    {
        T d = norm();
        if (d == T{0})
            return ZERO; // x = y = z = w = 0
        d = T{1} / d;
        return {x * d, y * d, z * d, w * d};
    }

    /**
     * Returns the conjugate of this quaternion.
     */
    Quaternion conjugated() const
    {
        return {-x, -y, -z, w};
    }

    /**
     * Returns a copy of this quaternion inverted (if possible).
     */
    Quaternion inverted() const
    {
        T d = norm();
        if (d == T{0})
            return ZERO; // x = y = z = w = 0
        d = T{-1} / d;
        return {x * d, y * d, z * d, -w * d};
    }

    /**
     * Returns the dot/scalar product of this and a given quaternion q.
     */
    T dot(const Quaternion& q) const
    {
        return x * q.x + y * q.y + z * q.z + w * q.w;
    }

    /**
     * Returns the rotation matrix corresponding to this object quaternion.
     */
    Matrix3x3<T> to_matrix3x3() const
    {
        const T d = norm();
        const T r = d == T{0} ? T{0} : T{2} / d;
        const T xs = x * r;
        const T ys = y * r;
        const T zs = z * r;
        const T xw = xs * w;
        const T yw = ys * w;
        const T zw = zs * w;
        const T xx = x * xs;
        const T xy = x * ys;
        const T xz = x * zs;
        const T yy = y * ys;
        const T yz = y * zs;
        const T zz = z * zs;
        return {
            T{1} - (yy + zz),   xy - zw,   xz + yw,
            xy + zw,   T{1} - (xx + zz),   yz - xw,
            xz - yw,   yz + xw,   T{1} - (xx + yy),
        };
    }

    /**
     * Extracts the basis vectors that define the transformed coordinate system.
     */
    void to_axes(Vector3<T>& x, Vector3<T>& y, Vector3<T>& z) const
    {
        to_matrix3x3().to_axes(x, y, z);
    }

    /**
     * Tries to extract the rotation axis and the rotation angle (in the range
     * [0;pi]) this quaternion is composed of.
     */
    bool to_axis_angle(Vector3<T>& axis, T& angle) const
    {
        const T length_squared = x*x + y*y + z*z;
        if (!(length_squared > T{0}))
        {
            axis = Vector3<T>::X; // Any axis would be "correct" here.
            angle = T{0};
            return false;
        }

        const T invlen = T{1} / std::sqrt(length_squared);
        axis.x = x * invlen;
        axis.y = y * invlen;
        axis.z = z * invlen;
        angle = T{2} * std::acos(w);
        return true;
    }

    /**
     * Assumes that this quaternion represents a "view" rotation for a left-handed
     * coordinate system and tries to extract the camera "look" direction and the
     * "up" direction.
     */
    bool is_look_lh(Vector3<T>& look, Vector3<T>& up) const
    {
        return to_matrix3x3().is_look_lh(look, up);
    }

    /**
     * Assumes that this quaternion represents a "view" rotation for a right-handed
     * coordinate system and tries to extract the camera "look" direction and the
     * "up" direction.
     */
    bool is_look_rh(Vector3<T>& look, Vector3<T>& up) const
    {
        return to_matrix3x3().is_look_rh(look, up);
    }

    /**
     * Returns a copy of the given vector transformed by this quaternion.
     */
    Vector3<T> transform(const Vector3<T>& v) const
    {
        Vector3<T> u{x, y, z};
        Vector3<T> uv = u.cross(v);
        Vector3<T> uuv = u.cross(uv);
        uv *= T{2} * w;
        uuv *= T{2};
        return v + uv + uuv;
    }

    /**
     * Returns a copy of the given vector transformed by this quaternion inverted
     * (if possible).
     */
    Vector3<T> transform_inverted(const Vector3<T>& v) const
    {
        T d = norm();
        if (d == T{0})
            return v;
        d = T{-1} / d;
        Vector3<T> u{x*d, y*d, z*d};
        Vector3<T> uv = u.cross(v);
        Vector3<T> uuv = u.cross(uv);
        uv *= T{2} * -w*d;
        uuv *= T{2};
        return v + uv + uuv;
    }



    // #############################################################################
    // Manipulations
    // #############################################################################

    /**
     * Normalizes this quaternion (if possible).
     */
    Quaternion& normalize()
    {
        *this = normalized();
        return *this;
    }

    /**
     * Conjugates this quaternion.
     */
    Quaternion& conjugate()
    {
        x = -x;
        y = -y;
        z = -z;
        return *this;
    }

    /**
     * Inverts this quaternion (if possible).
     */
    Quaternion& invert()
    {
        *this = inverted();
        return *this;
    }



    // #############################################################################
    // Comparison Operators
    // #############################################################################

    bool operator==(const Quaternion& q) const noexcept { return (x == q.x) && (y == q.y) && (z == q.z) && (w == q.w); }
    bool operator!=(const Quaternion& q) const noexcept { return (x != q.x) || (y != q.y) || (z != q.z) || (w != q.w); }



    // #############################################################################
    // Arithmetic Operators
    // #############################################################################

    /** Returns a copy of this quaternion unmodified. */
    constexpr Quaternion operator+() const { return *this; }
    /** Returns a copy of this quaternion with the signs of the components flipped. */
    constexpr Quaternion operator-() const { return {-x, -y, -z, -w}; }

    /** Returns the (component-wise) sum of two quaternions. */
    constexpr Quaternion operator+(const Quaternion& v) const { return {x + v.x, y + v.y, z + v.z, w + v.w}; }
    /** Returns the (component-wise) difference of two quaternions. */
    constexpr Quaternion operator-(const Quaternion& v) const { return {x - v.x, y - v.y, z - v.z, w - v.w}; }

    /** Returns a copy of this quaternion "scaled" by the specified factor (scalar multiplication). */
    constexpr Quaternion operator*(T s) const { return {x * s, y * s, z * s, w * s}; }
    /** Returns a copy of this quaternion inversely "scaled" by the specified factor (scalar division). */
    constexpr Quaternion operator/(T s) const { return {x / s, y / s, z / s, w / s}; }

    /** Returns the quaternion product of two quaternions (quaternion multiplication). */
    constexpr Quaternion operator*(const Quaternion& q) const
    {
        return {
            w*q.x + x*q.w + y*q.z - z*q.y,
            w*q.y + y*q.w + z*q.x - x*q.z,
            w*q.z + z*q.w + x*q.y - y*q.x,
            w*q.w - x*q.x - y*q.y - z*q.z,
        };
    }

    /** Returns a copy of the given vector transformed by this quaternion. */
    Vector3<T> operator*(const Vector3<T>& v) const { return transform(v); }

    /** "Scales" a quaternion by the specified factor (scalar multiplication). */
    friend constexpr Quaternion operator*(T s, const Quaternion& q) { return q * s; }



    // #############################################################################
    // Arithmetic Assignment Operators
    // #############################################################################

    /** Adds the given quaternion to this one, resulting in the (component-wise) quaternion sum. */
    constexpr Quaternion& operator+=(const Quaternion& v) { x += v.x; y += v.y; z += v.z; w += v.w; return *this; }
    /** Subtracts the given quaternion from this one, resulting in the (component-wise) quaternion difference. */
    constexpr Quaternion& operator-=(const Quaternion& v) { x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this; }

    /** "Scales" this quaternion by the specified factor (scalar multiplication). */
    constexpr Quaternion& operator*=(T s) { x *= s; y *= s; z *= s; w *= s; return *this; }
    /** Inversely "scales" this quaternion by the specified factor (scalar division). */
    constexpr Quaternion& operator/=(T s) { x /= s; y /= s; z /= s; w /= s; return *this; }



    // #############################################################################
    // Access Operators
    // #############################################################################

    T operator[](unsigned i) const { assert(i < 4); return data[i]; }
    T& operator[](unsigned i) { assert(i < 4); return data[i]; }



    // #############################################################################
    // Static Functions
    // #############################################################################

    /**
     * Initializes a quaternion from the specified rotation matrix.
     */
    static constexpr Quaternion from_matrix3x3(const Matrix3x3<T>& m)
    {
        return Quaternion(m);
    }

    /**
     * Initializes a quaternion from the specified basis vectors that define the
     * transformed coordinate system.
     */
    static constexpr Quaternion from_axes(const Vector3<T>& x, const Vector3<T>& y, const Vector3<T>& z)
    {
        return from_matrix3x3(Matrix3x3<T>::from_axes(x, y, z));
    }

    /**
     * Initializes a quaternion representing a rotation by a certain angle (in
     * radians) around the x-axis.
     */
    static Quaternion rotx(T angle)
    {
        const T sn05 = std::sin(angle * T{0.5});
        const T cs05 = std::cos(angle * T{0.5});
        return {
            sn05,
            T{0},
            T{0},
            cs05,
        };
    }

    /**
     * Initializes a quaternion representing a rotation by a certain angle (in
     * radians) around the y-axis.
     */
    static Quaternion roty(T angle)
    {
        const T sn05 = std::sin(angle * T{0.5});
        const T cs05 = std::cos(angle * T{0.5});
        return {
            T{0},
            sn05,
            T{0},
            cs05,
        };
    }

    /**
     * Initializes a quaternion representing a rotation by a certain angle (in
     * radians) around the z-axis.
     */
    static Quaternion rotz(T angle)
    {
        const T sn05 = std::sin(angle * T{0.5});
        const T cs05 = std::cos(angle * T{0.5});
        return {
            T{0},
            T{0},
            sn05,
            cs05,
        };
    }

    /**
     * Initializes a quaternion representing a rotation around the specified axis,
     * with an angle equal to the magnitude of the specified vector (in radians).
     */
    static Quaternion rotation(const Vector3<T>& axis_angle)
    {
        const T angle = axis_angle.length();
        if (angle == T{0})
            return IDENTITY;
        return rotation_normalized(axis_angle / angle, angle);
    }

    /**
     * Initializes a quaternion representing a rotation by a certain angle (in
     * radians) around the specified axis.
     */
    static Quaternion rotation(const Vector3<T>& axis, T angle)
    {
        if (angle == T{0})
            return IDENTITY;
        return rotation_normalized(axis.normalized(), angle);
    }

    /**
     * Initializes a quaternion representing a rotation by a certain angle (in
     * radians) around an axis specified as a normalized unit vector.
     */
    static Quaternion rotation_normalized(const Vector3<T>& unit_axis, T angle)
    {
        const T sn05 = std::sin(angle * T{0.5});
        const T cs05 = std::cos(angle * T{0.5});
        return {
            unit_axis.x * sn05,
            unit_axis.y * sn05,
            unit_axis.z * sn05,
            cs05,
        };
    }

    /**
     * Initializes a quaternion representing a "view" rotation for a left-handed
     * coordinate system using a camera "look" direction and an "up" direction.
     */
    static Quaternion look_lh(const Vector3<T>& look, const Vector3<T>& up)
    {
        return from_matrix3x3(Matrix3x3<T>::look_lh(look, up));
    }

    /**
     * Initializes a quaternion representing a "view" rotation for a right-handed
     * coordinate system using a camera "look" direction and an "up" direction.
     */
    static Quaternion look_rh(const Vector3<T>& look, const Vector3<T>& up)
    {
        return from_matrix3x3(Matrix3x3<T>::look_rh(look, up));
    }

}; // struct Quaternion



    template <std::floating_point T>
    const Quaternion<T> Quaternion<T>::ZERO =     {T{0}, T{0}, T{0}, T{0}};
    template <std::floating_point T>
    const Quaternion<T> Quaternion<T>::IDENTITY = {T{0}, T{0}, T{0}, T{1}};



using QuaternionF = Quaternion<float>;
using QuaternionD = Quaternion<double>;
using QuaternionLD = Quaternion<long double>;

//extern template class Quaternion<float>;
//extern template class Quaternion<double>;



} // namespace ml7

#endif // ML7_QUATERNION_H
