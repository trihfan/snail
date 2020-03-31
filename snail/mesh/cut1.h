/*
 * Cut a triangle with one intersections
 */

#pragma once

namespace snail
{
    template <typename type>
    class cut1
	{
    public:
        /**
         * @brief Cut a triangle with one intersections
         */
        static void cut(mesh<type>* mesh, const std::vector<intersection<type>>& intersections, const triangle<type>& triangleToCut);

    private:
        /**
         * @brief Intersection hint used to classify a cut
         */
        using intersectionHint = typename triangleTriangleIntersection<type>::intersectionHint;
	};


    template <typename type>
    void cut1<type>::cut(mesh<type>* mesh, const std::vector<intersection<type>>& intersections, const triangle<type>& triangleToCut)
    {
        size_t intersection = mesh->addVertex(intersections[0].position);

        switch (intersections[0].hint)
        {
        case intersectionHint::ab:
        case intersectionHint::bc:
        case intersectionHint::ac:
            mesh->addTriangle(triangleToCut[(intersections[0].hint) % 3], intersection, triangleToCut[(2 + intersections[0].hint) % 3]);
            mesh->addTriangle(triangleToCut[(1 + intersections[0].hint) % 3], triangleToCut[(2 + intersections[0].hint) % 3], intersection);
            break;

        case intersectionHint::inside:
            mesh->addTriangle(triangleToCut[0], triangleToCut[1], intersection);
            mesh->addTriangle(triangleToCut[1], triangleToCut[2], intersection);
            mesh->addTriangle(triangleToCut[2], triangleToCut[0], intersection);
            break;
        }
    }
}
