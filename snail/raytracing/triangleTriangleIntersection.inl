template <typename type>
bool triangleTriangleIntersection<type>::intersects(const triangle<type>& triangleA, std::vector<intersection<type>>& intersectionA,
                                                    const triangle<type>& triangleB, std::vector<intersection<type>>& intersectionB)
{
    log::info << "TRIANGLE INTERSECTION START";
    log::info << "triangle 1: " << triangleA;
    log::info << "triangle 2: " << triangleB;

    std::vector<intersection<type>> fullA, fullB;

    // Compute intersections
    if (!intersects(triangleA, triangleB, fullA))
    {
        return false;
    }
    intersects(triangleB, triangleA, fullB);

    // Fill
    extractIntersections(fullA, intersectionA, fullB, intersectionB);

    log::info << "TRIANGLE INTERSECTION END" << std::endl;
    return !intersectionA.empty() or !intersectionB.empty();
}

template <typename type>
void triangleTriangleIntersection<type>::extractIntersections(const std::vector<intersection<type>>& inputA, std::vector<intersection<type>>& outputA,
                                                              const std::vector<intersection<type>>& inputB, std::vector<intersection<type>>& outputB)
{
    outputA = inputA;
    outputB = inputB;

    for (const intersection<type>& inter : inputB)
    {
        if (notA(inter) and notB(inter) and notC(inter) and std::find_if(outputA.begin(), outputA.end(), [&inter](const intersection<type>& interA){ return equalsV(inter.position, interA.position, ratioEpsilon<type>()); }) == outputA.end())
        {
            outputA.push_back(inter);
            bool uEquals0 = equals(inter.u, type(0));
            bool vEquals0 = equals(inter.v, type(0));

            if (uEquals0 and !vEquals0)
            {
                outputA.back().hint = ac;
            }
            else if (!uEquals0 and vEquals0)
            {
                outputA.back().hint = ab;
            }
            else if (equals(inter.u + inter.v, type(1)))
            {
                outputA.back().hint = bc;
            }
            else
            {
                outputA.back().hint = inside;
            }
        }
    }

    for (const intersection<type>& inter : inputA)
    {
        if (notA(inter) and notB(inter) and notC(inter) and std::find_if(outputB.begin(), outputB.end(), [&inter](const intersection<type>& interB){ return equalsV(inter.position, interB.position, ratioEpsilon<type>()); }) == outputB.end())
        {
            outputB.push_back(inter);
            bool uEquals0 = equals(inter.u, type(0));
            bool vEquals0 = equals(inter.v, type(0));

            if (uEquals0 and !vEquals0)
            {
                outputB.back().hint = ac;
            }
            else if (!uEquals0 and vEquals0)
            {
                outputB.back().hint = ab;
            }
            else if (equals(inter.u + inter.v, type(1)))
            {
                outputB.back().hint = bc;
            }
            else
            {
                outputB.back().hint = inside;
            }
        }
    }

    for (size_t i = 0; i < outputA.size(); i++)
    {
        if (outputA[i].hint > inside)
        {
            outputA.erase(outputA.begin() + i);
            i--;
        }
    }

    for (size_t i = 0; i < outputB.size(); i++)
    {
        if (outputB[i].hint > inside)
        {
            outputB.erase(outputB.begin() + i);
            i--;
        }
    }

    if (outputA.size() > 3 or outputB.size() > 3)
    {
        log::err << "more than 2 intersections";
    }

    if (!outputA.empty() and !outputB.empty())
    {
        log::info << "Input A: " << inputA;
        log::info << "Input B: " << inputB;
        log::info << "Output A: " << outputA;
        log::info << "Output B: " << outputB;
    }
}

template <typename type>
bool triangleTriangleIntersection<type>::intersects(const triangle<type>& triangleA, const triangle<type>& triangleB, std::vector<intersection<type>>& intersections)
{
    size_t nbParallels = 0;

    // Intersect node a sides with node b
    for (size_t i = 0; i < 3; i++)
    {
        type t, u, v;
        auto result = triangleRayIntersection<type>::intersects(triangleA.getSide(i), triangleB.getSide(0).getOrigin(), triangleB.getSide(1).getOrigin(), triangleB.getSide(2).getOrigin(), t, u, v);
        static const std::array<std::string, 3> resultToStr { "parallels", "outside", "inside" };
        log::info << "intersects: r=" << resultToStr[result] << ", u=" << u << ", v=" << v << ", t=" << t;

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
            if (percent < ratioEpsilon<type>())
            {
                hint = static_cast<intersectionHint>(i + 4);
            }
            else if (percent > type(1 - ratioEpsilon<type>()))
            {
                hint = static_cast<intersectionHint>(((i + 1) % 3) + 4);
            }
            else
            {
                hint = static_cast<intersectionHint>(i);
            }

            auto vertex = triangleA.getSide(i).getOrigin() + triangleA.getSide(i).getDirection() * type(t);
            log::info << "origin=" << triangleA.getSide(i).getOrigin() << ", unit=" << triangleA.getSide(i).getDirection() << ", t=" << t << ", l=" << triangleA.getSide(i).getLength();
            intersections.push_back({ hint, vertex, t, u, v });
        }
    }

    return true;
}
