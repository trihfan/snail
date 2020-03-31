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

    template <typename type>
    typename triangleRayIntersection<type>::intersectionHint triangleRayIntersection<type>::intersects(
        const ray<type>& ray, const vector3<type>& a, const vector3<type>& b, const vector3<type>& c, type& t, type& u, type& v)
    {
        vector3<type> ab = b - a;
        vector3<type> ac = c - a;
        vector3<type> pvec = ray.getDirection().cross(ac);
        type det = ab.dot(pvec);

        // side and triangle are parallel if det is close to 0
        if (isNull(det))
        {
            return parallels;
        }

        type invDet = type(1) / det;

        // compute u
        vector3<type> tvec = ray.getOrigin() - a;
        u = tvec.dot(pvec) * invDet;

        // Check u
        if (u < -ratioEpsilon<type>() or u > type(1) + ratioEpsilon<type>())
        {
            return outside;
        }

        // compute v
        vector3<type> qvec = tvec.cross(ab);
        v = ray.getDirection().dot(qvec) * invDet;

        // Check v
        if (v < -ratioEpsilon<type>() or u + v > type(1) + ratioEpsilon<type>())
        {
            return outside;
        }

        // compute t
        t = ac.dot(qvec) * invDet;

        // Check t
        type ratio = t / ray.getLength();
        if (ratio < -ratioEpsilon<type>() or ratio > type(1) + ratioEpsilon<type>())
        {
            return outside;
        }

        assert(!std::isnan(t));
        return inside;
    }
}
