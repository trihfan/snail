/*
 * Cut a triangle with two intersections
 */

#pragma once

namespace snail
{
    template <typename type>
    class cut2
    {
    public:
        /**
         * @brief Cut a triangle with two intersections
         */
        static void cut(mesh<type>* mesh, const std::vector<intersection<type>>& intersections, const triangle<type>& triangleToCut);

    private:
        /**
         * @brief Intersection hint used to classify a cut
         */
        using intersectionHint = typename triangleTriangleIntersection<type>::intersectionHint;

        /**
         * @brief Cut a triangle with two intersections inside the triangle
         */
        static void cutInsideInside(mesh<type>* mesh, const std::vector<intersection<type>>& intersections, const triangle<type>& triangleToCut);

        /**
         * @brief Cut a triangle with one intersection inside and one one the side of the triangle
         */
        static void cutInsideSide(mesh<type>* mesh, const std::vector<intersection<type>>& intersections, const triangle<type>& triangleToCut);

        /**
         * @brief Cut a triangle with two intersections on the same side
         */
        static void cutSameSide(mesh<type>* mesh, const std::vector<intersection<type>>& intersections, const triangle<type>& triangleToCut);

        /**
         * @brief Cut a triangle with two intersections on separates sides of the triangle
         */
        static void cutTwoSides(mesh<type>* mesh, const std::vector<intersection<type>>& intersections, const triangle<type>& triangleToCut);
	};

    template <typename type>
    void cut2<type>::cut(mesh<type>* mesh, const std::vector<intersection<type>>& intersections, const triangle<type>& triangleToCut)
    {
        // inside inside
        if (intersections[0].hint == intersectionHint::inside && intersections[1].hint == intersectionHint::inside)
        {
            cutInsideInside(mesh, intersections, triangleToCut);
        }
        // inside side
        else if (intersections[1].hint == intersectionHint::inside)
        {
            cutInsideSide(mesh, intersections, triangleToCut);
        }
        // same side
        else if(intersections[0].hint == intersections[1].hint)
        {
            cutSameSide(mesh, intersections, triangleToCut);
        }
        // two sides
        else
        {
            cutTwoSides(mesh, intersections, triangleToCut);
        }
    }

    template <typename type>
    void cut2<type>::cutInsideInside(mesh<type>* mesh, const std::vector<intersection<type>>& intersections, const triangle<type>& triangleToCut)
    {
        cut1<type>::cut(mesh, intersections, triangleToCut);
        return; // todo

        std::array<uint64_t, 2> indices
        {
            mesh->addVertex(intersections[0].position),
            mesh->addVertex(intersections[1].position)
        };

        size_t a = triangleToCut[0];
        size_t b = triangleToCut[1];
        size_t c = triangleToCut[2];
    }

    template <typename type>
    void cut2<type>::cutInsideSide(mesh<type>* mesh, const std::vector<intersection<type>>& intersections, const triangle<type>& triangleToCut)
    {
        std::array<uint64_t, 2> indices
        {
            mesh->addVertex(intersections[0].position),
            mesh->addVertex(intersections[1].position)
        };

        size_t a = triangleToCut[intersections[0].hint];
        size_t b = triangleToCut[(intersections[0].hint + 1) % 3];
        size_t c = triangleToCut[(intersections[0].hint + 2) % 3];

        mesh->addTriangle(a, indices[0], indices[1]);
        mesh->addTriangle(indices[0], b, indices[1]);
        mesh->addTriangle(b, c, indices[1]);
        mesh->addTriangle(a, indices[1], c);
    }

    template <typename type>
    void cut2<type>::cutSameSide(mesh<type>* mesh, const std::vector<intersection<type>>& intersections, const triangle<type>& triangleToCut)
    {
        std::array<uint64_t, 2> indices
        {
            mesh->addVertex(intersections[0].position),
            mesh->addVertex(intersections[1].position)
        };

        size_t intersection_1 = (intersections[0].position - triangleToCut.getSide(intersections[0].hint).getOrigin()).squaredNorm() >
                                (intersections[1].position - triangleToCut.getSide(intersections[0].hint).getOrigin()).squaredNorm() ?
                                0 : 1;

        switch (intersections[0].hint)
        {
        case intersectionHint::ab:
            intersection_1 = intersections[0].u < intersections[1].u ? 0 : 1;
            break;

        case intersectionHint::bc:
            intersection_1 = intersections[0].u > intersections[1].u ? 0 : 1;
            break;

        case intersectionHint::ac:
            intersection_1 = intersections[0].v > intersections[1].v ? 0 : 1;
            break;

        default:
            assert(false);
            break;
        }

        size_t intersection_2 = intersection_1 == 0 ? 1 : 0;
        size_t a = triangleToCut[intersections[0].hint];
        size_t b = triangleToCut[(intersections[0].hint + 1) % 3];
        size_t c = triangleToCut[(intersections[0].hint + 2) % 3];

        mesh->addTriangle(a, indices[intersection_1], c);
        mesh->addTriangle(indices[intersection_1], indices[intersection_2], c);
        mesh->addTriangle(indices[intersection_2], b, c);
    }

    template <typename type>
    void cut2<type>::cutTwoSides(mesh<type>* mesh, const std::vector<intersection<type>>& intersections, const triangle<type>& triangleToCut)
    {
        std::array<uint64_t, 2> indices
        {
            mesh->addVertex(intersections[0].position),
            mesh->addVertex(intersections[1].position)
        };

        size_t first_side = intersections[0].hint == intersectionHint::ab ? (intersections[1].hint == intersectionHint::bc ? 0 : 2) : 1;
        size_t a = triangleToCut[first_side];
        size_t b = triangleToCut[(first_side + 1) % 3];
        size_t c = triangleToCut[(first_side + 2) % 3];

        size_t intersection_1 = first_side == 2 ? 1 : 0;
        size_t intersection_2 = intersection_1 == 0 ? 1 : 0;

        bool link_to_a = (intersections[0].position - triangleToCut.getSide(first_side).getOrigin()).squaredNorm() >
                         (intersections[1].position - triangleToCut.getSide((first_side + 2) % 3).getOrigin()).squaredNorm();

        mesh->addTriangle(indices[intersection_1], b, indices[intersection_2]);
        mesh->addTriangle(a, indices[intersection_1], link_to_a ? indices[intersection_2] : c);
        mesh->addTriangle(link_to_a ? a : indices[intersection_1], indices[intersection_2], c);
    }
}
