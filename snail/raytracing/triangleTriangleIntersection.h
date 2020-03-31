/*
 * Compute intersection of two triangles
 */

#pragma once

namespace snail
{
    template <typename type>
    class triangleTriangleIntersection
    {
    public:
        /**
         * @brief Hint to classify the intersection
         */
        enum intersectionHint : int { ab, bc, ac, inside, a, b, c };

        /**
         * @brief An intersection returned by the intersects method
         */
        struct intersection
        {
            intersectionHint hint;
            vector3<type> position;
            type t, u, v;
        };

        static bool intersects(const triangle<type>& triangleA, std::vector<intersection>& intersectionA,
                               const triangle<type>& triangleB, std::vector<intersection>& intersectionB);

    private:
        static bool intersects(const triangle<type>& triangleA, const triangle<type>& triangleB, std::vector<intersection>& intersections);

        static void extractIntersections(const std::vector<intersection>& inputA, std::vector<intersection>& outputA,
                                         const std::vector<intersection>& inputB, std::vector<intersection>& outputB);
    };

    template <typename type>
    using intersection = typename triangleTriangleIntersection<type>::intersection;

    template <typename type>
    bool triangleTriangleIntersection<type>::intersects(const triangle<type>& triangleA, std::vector<intersection>& intersectionA,
                                                        const triangle<type>& triangleB, std::vector<intersection>& intersectionB)
    {
        std::vector<intersection> fullA, fullB;

        // Compute intersections
        if (!intersects(triangleA, triangleB, fullA))
        {
            return false;
        }
        intersects(triangleB, triangleA, fullB);

        // Fill
        extractIntersections(fullA, intersectionA, fullB, intersectionB);

        return true;
    }

    template <typename type>
    void triangleTriangleIntersection<type>::extractIntersections(const std::vector<intersection>& inputA, std::vector<intersection>& outputA,
                                                                  const std::vector<intersection>& inputB, std::vector<intersection>& outputB)
    {
        for (const auto& inter : inputA)
        {
            if (inter.hint <= ac)
            {
                outputA.push_back(inter);
            }
        }

        for (const auto& inter : inputB)
        {
            if (inter.hint <= ac)
            {
                outputB.push_back(inter);
            }
        }

        for (const auto& inter : inputB)
        {
            if (inter.u > epsilon<type>() and inter.v > epsilon<type>() and inter.u + inter.v < type(1) - epsilon<type>())
            {
                outputA.push_back(inter);
                outputA.back().hint = inside;
            }
        }

        for (const auto& inter : inputA)
        {
            if (inter.u > epsilon<type>() and inter.v > epsilon<type>() and inter.u + inter.v < type(1) - epsilon<type>())
            {
                outputB.push_back(inter);
                outputB.back().hint = inside;
            }
        }

        assert(outputA.size() < 3);
        assert(outputB.size() < 3);
    }

    template <typename type>
    bool triangleTriangleIntersection<type>::intersects(const triangle<type>& triangleA, const triangle<type>& triangleB, std::vector<intersection>& intersections)
    {
        size_t nbParallels = 0;

        // Intersect node a sides with node b
        for (size_t i = 0; i < 3; i++)
        {
            type t, u, v;
            auto result = triangleRayIntersection<type>::intersects(triangleA.getSide(i), triangleB.getSide(0).getOrigin(), triangleB.getSide(1).getOrigin(), triangleB.getSide(2).getOrigin(), t, u, v);

            // Check the intersection result
            if (result == triangleRayIntersection<type>::parallels)
            {
                if (++nbParallels > 1)
                {
                    // Coplanar
                    return false;
                }
            }
            else if (result == triangleRayIntersection<type>::inside)
            {
                intersectionHint hint;
                type percent = t / triangleA.getSide(i).getLength();
                if (percent < type(1e-3))
                {
                    hint = static_cast<intersectionHint>(i + 4);
                }
                else if (percent > type(1 - 1e-3))
                {
                    hint = static_cast<intersectionHint>(((i + 1) % 3) + 4);
                }
                else
                {
                    hint = static_cast<intersectionHint>(i);
                }

                auto vertex = triangleA.getSide(i).getOrigin() + triangleA.getSide(i).getDirection() * type(t);
                intersections.push_back({ hint, vertex, t, u, v });
            }
        }

        return true;
    }
}
