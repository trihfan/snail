/*
 * Math helpers for snail
 */

#pragma once

namespace snail
{
    // Constants
    template <typename type>
    constexpr type pi()
    {
        return std::atan(static_cast<type>(1)) * static_cast<type>(4);
    }

    template <typename type>
    constexpr type pi2()
    {
        return snail::pi<type>() / static_cast<type>(2);
    }

    template <typename type>
    constexpr type epsilon()
    {
        return type(1e-6);
    }

    template <typename type>
    constexpr type ratioEpsilon()
    {
        return type(1e-4);
    }

    // Trigo
    template <typename type>
    type cos(type radians)
    {
        return std::cos(radians);
    }

    template <typename type>
    type sin(type radians)
    {
        return std::sin(radians);
    }

    template <typename type>
    type tan(type radians)
    {
        return std::tan(radians);
    }

    // Conversion
    template <typename type>
    type deg2rad(type degrees)
    {
        return degrees * snail::pi<type>() / type(180);
    }


    template <typename type>
    type rad2deg(type radians)
    {
        return radians * type(180) / snail::pi<type>();
    }

    // Comparison
    template <typename type>
    bool equals(const type& v1, const type& v2, const type& eps = epsilon<type>())
    {
        return std::abs(v1 - v2) < eps;
    }

    template <typename type>
    bool equalsV(const vector3<type>& v1, const vector3<type>& v2, const type& eps = epsilon<type>())
    {
        return equals(v1.x(), v2.x(), eps) and equals(v1.y(), v2.y(), eps) and equals(v1.z(), v2.z(), eps);
    }

    template <typename type>
    bool isNull(type v1)
    {
        return std::abs(v1) < snail::epsilon<type>();
    }
}
