/*
 * Compute intersection of two triangles
 */

#pragma once

namespace snail
{
    /**
     * @brief Hint to classify the intersection
     */
    enum intersectionHint : int { ab, bc, ac, inside, a, b, c };

    /**
     * @brief An intersection returned by the intersects method
     */
    template <typename type>
    struct intersection
    {
        intersectionHint hint;
        vector3<type> position;
        type t, u, v;
    };

    template <typename type>
    class triangleTriangleIntersection
    {
    public:
        static bool intersects(const triangle<type>& triangleA, std::vector<intersection<type>>& intersectionA,
                               const triangle<type>& triangleB, std::vector<intersection<type>>& intersectionB);

    private:
        static bool intersects(const triangle<type>& triangleA, const triangle<type>& triangleB, std::vector<intersection<type>>& intersections);

        static void extractIntersections(const std::vector<intersection<type>>& inputA, std::vector<intersection<type>>& outputA,
                                         const std::vector<intersection<type>>& inputB, std::vector<intersection<type>>& outputB);

        static bool notA(const intersection<type>& inter)
        {
            return inter.u > ratioEpsilon<type>() or inter.v > ratioEpsilon<type>();
        }

        static bool notB(const intersection<type>& inter)
        {
            return inter.u < (type(1) - ratioEpsilon<type>());
        }

        static bool notC(const intersection<type>& inter)
        {
            return inter.v < (type(1) - ratioEpsilon<type>());
        }
    };

    #include "triangleTriangleIntersection.inl"
}
