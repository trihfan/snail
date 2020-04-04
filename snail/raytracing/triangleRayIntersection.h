/*
 * Compute intersection of a triangle with a ray
 */

#pragma once

namespace snail
{
    template <typename type>
    class triangleRayIntersection
    {
    public:
        /**
         * @brief Hint to classify the intersection
         */
        enum intersectionHint : int
        {
            parallels, outside, inside
        };

        /**
         * @brief Compute the intersection between a triangle side and a triangle using moller trumbore algorithm
         */
        static intersectionHint intersects(const ray<type>& ray, const vector3<type>& a, const vector3<type>& b, const vector3<type>& c, type& t, type& u, type& v);
    };

    #include "triangleRayIntersection.inl"
}
