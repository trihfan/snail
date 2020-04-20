template <typename type>
mesh<type>::mesh(size_t reserveSize)
    : id(currentMeshIndex<type>++)
{
    vertices.reserve(reserveSize);
    triangles.reserve(reserveSize / 3);
}

template <typename type>
mesh<type>::~mesh()
{
    clear();
}

template <typename type>
void mesh<type>::clear()
{
    for (auto t : triangles)
    {
        delete t;
    }

    vertices.clear();
    verticesMap.clear();
    triangles.clear();
}

template <typename type>
mesh<type>* mesh<type>::clone() const
{
    auto clone = new mesh<type>();

    // Vertices
    clone->vertices = vertices;
    for (const vector3<type>& vertex : clone->vertices)
    {
        clone->verticesMap[vertex] = vertex.getIndex();
    }

    // Triangles
    clone->triangles.resize(triangles.size());
    for (size_t i = 0; i < triangles.size(); i++)
    {
        size_t a = (*triangles[i])[0];
        size_t b = (*triangles[i])[1];
        size_t c = (*triangles[i])[2];

        ray<type> ab(vertices[a], vertices[b]);
        ray<type> bc(vertices[b], vertices[c]);
        ray<type> ca(vertices[c], vertices[a]);

        clone->triangles[i] = new triangle<type>(a, b, c, ab, bc, ca);
        clone->triangles[i]->setIndex(i);
        clone->triangles[i]->setId(clone->id);
    }

    return clone;
}

template <typename type>
size_t mesh<type>::addVertex(const vector3<type>& vertex)
{
    auto it = verticesMap.find(vertex);
    if (it != verticesMap.end())
    {
        return it->second;
    }

    size_t index = vertices.size();
    vertices.push_back(vertex);
    vertices[index].setIndex(index);
    verticesMap[vertex] = index;
    return index;
}

template <typename type>
void mesh<type>::removeVertex(const vector3<type>& vertex)
{
    removeVertex(vertex.getIndex());
}

template <typename type>
void mesh<type>::removeVertex(size_t index)
{
    const size_t lastIndex = vertices.size() - 1;
    if (index != lastIndex)
    {
        vertices[index] = std::move(vertices[lastIndex]);
        vertices[index].setIndex(index);
        verticesMap[vertices[index]] = index;

        for (auto& triangle : triangles)
        {
            for (size_t i = 0; i < 3; i++)
            {
                if (triangle[i] == lastIndex)
                {
                    triangle[i] = index;
                }
            }
        }
    }
    vertices.pop_back();
}

template <typename type>
size_t mesh<type>::getVertexCount() const
{
    return vertices.size();
}

template <typename type>
const vector3<type>& mesh<type>::getVertex(size_t index) const
{
    return vertices[index];
}

template <typename type>
size_t mesh<type>::addTriangle(size_t a, size_t b, size_t c)
{
    if (a == b or b == c or a == c)
    {
        log(err) << "add triangle with not 3 separates indices";
        return std::numeric_limits<size_t>::max();
    }

    // Compute sides
    ray<type> ab(vertices[a], vertices[b]);
    ray<type> bc(vertices[b], vertices[c]);
    ray<type> ca(vertices[c], vertices[a]);

    triangle<type>* t = new triangle<type>(a, b, c, ab, bc, ca);

    if (t->getArea() < epsilon<type>())
    {
        delete t;
        return std::numeric_limits<size_t>::max();
    }

    size_t index = triangles.size();
    triangles.push_back(t);
    t->setIndex(index);
    t->setId(id);
    return index;
}

template <typename type>
size_t mesh<type>::addTriangle(const vector3<type>& a, const vector3<type>& b, const vector3<type>& c)
{
    return addTriangle(addVertex(a), addVertex(b), addVertex(c));
}

template <typename type>
void mesh<type>::removeTriangle(const triangle<type>& triangle)
{
    remove(triangle.getIndex());
}

template <typename type>
void mesh<type>::removeTriangle(size_t index)
{
    if (index != triangles.size() - 1)
    {
        std::swap(triangles[index], triangles.back());
        triangles[index]->setIndex(index);
    }
    delete triangles.back();
    triangles.pop_back();
}

template <typename type>
size_t mesh<type>::getTriangleCount() const
{
    return triangles.size();
}

template <typename type>
const triangle<type>& mesh<type>::getTriangle(size_t index) const
{
    return *triangles[index];
}

template <typename type>
void mesh<type>::add(const mesh<type>* m)
{
    compute(booleanOperation::addition, std::unique_ptr<mesh<type>>(m->clone()));
}

template <typename type>
void mesh<type>::add(std::unique_ptr<mesh<type>> m)
{
    compute(booleanOperation::addition, std::move(m));
}

template <typename type>
void mesh<type>::sub(const mesh<type>* m)
{
    compute(booleanOperation::difference, std::unique_ptr<mesh<type>>(m->clone()));
}

template <typename type>
void mesh<type>::sub(std::unique_ptr<mesh<type>> m)
{
    compute(booleanOperation::difference, std::move(m));
}

template <typename type>
void mesh<type>::inter(const mesh<type>* m)
{
    compute(booleanOperation::intersection, std::unique_ptr<mesh<type>>(m->clone()));
}

template <typename type>
void mesh<type>::inter(std::unique_ptr<mesh<type>> m)
{
    compute(booleanOperation::intersection, std::move(m));
}

template <typename type>
void mesh<type>::compute(booleanOperation operation, std::unique_ptr<mesh<type>> other)
{
    // cut triangles
    cutMesh(other.get());

    // merge
    mergeMesh(operation, other.get());

    // refine
    refine();
}

template <typename type>
void mesh<type>::cutMesh(mesh<type>* other)
{
    std::array<mesh<type>*, 2> meshes { this, other };

    std::list<triangle<type>*> triangleToCut;
    size_t numberOfIterations = 0;
    size_t numberOfSlices = 0;

    bool work = true;
    while (work)
    {
        // Fill work list
        if (work)
        {
            for (mesh<type>* m : meshes)
            {
                for (triangle<type>* t : m->triangles)
                {
                    if (!t->hasFlag(triangle<type>::invalid))
                    {
                        triangleToCut.push_back(t);
                    }
                }
            }
            work = false;
        }

        // Compute each triangle in the list
        size_t currentNumberOfSlices = numberOfSlices;
        while (!triangleToCut.empty())
        {
            triangle<type>* current = triangleToCut.front();
            triangleToCut.pop_front();
            if (current->hasFlag(triangle<type>::invalid))
            {
                continue;
            }

            size_t meshAIndex = current->getId() == id ? 0 : 1;
            size_t meshBIndex = meshAIndex == 0 ? 1 : 0;

            for (const triangle<type>* otherTriangle : meshes[meshBIndex]->triangles)
            {
                if (otherTriangle->hasFlag(triangle<type>::invalid))
                {
                    continue;
                }

                std::vector<intersection<type>> intersectionA;
                std::vector<intersection<type>> intersectionB;

                if (triangleTriangleIntersection<type>::intersects(*current, intersectionA, *otherTriangle, intersectionB))
                {
                    log(debug) << "START CUT";
                    log(debug) << "triangle 1: " << *current;
                    log(debug) << "triangle 2: " << *otherTriangle;

                    bool error = false;
                    if (!intersectionA.empty())
                    {
                        if (!meshes[meshAIndex]->cutTriangle(current->getIndex(), intersectionA[0]))
                        {
                            error = true;
                        }
                        else
                        {
                            numberOfSlices++;
                        }
                    }

                    if (!intersectionB.empty())
                    {
                        if (!meshes[meshBIndex]->cutTriangle(otherTriangle->getIndex(), intersectionB[0]))
                        {
                            error = true;
                        }
                        else
                        {
                            numberOfSlices++;
                        }
                    }

                    log(debug) << "END CUT" << std::endl;

                    // debug export
                    #if VERBOSITY_LEVEL >= 3
                    if (error)
                    {
                        // get file index
                        static size_t index = 0;
                        index++;
                        std::string scriptFilename = std::string(DEBUG_OUTPUT_DIRECTORY) + "debug" + std::to_string(index);

                        // mesh 1
                        std::string mesh1Filename = scriptFilename + "_mesh1";
                        std::ofstream mesh1File(mesh1Filename);
                        mesh<type> tmp1;
                        tmp1.addTriangle(current->getSide(0).getOrigin(), current->getSide(1).getOrigin(), current->getSide(2).getOrigin());
                        mesh1File << nlohmann::json(tmp1);

                        // mesh 2
                        std::string mesh2Filename = scriptFilename + "_mesh2";
                        std::ofstream mesh2File(mesh2Filename);
                        mesh<type> tmp2;
                        tmp2.addTriangle(otherTriangle->getSide(0).getOrigin(), otherTriangle->getSide(1).getOrigin(), otherTriangle->getSide(2).getOrigin());
                        mesh2File << nlohmann::json(tmp2);

                        // script
                        std::ofstream scriptFile(scriptFilename);
                        scriptFile << "var mesh1 = load(\"" << mesh1Filename << "\")" << std::endl;
                        scriptFile << "var mesh2 = load(\"" << mesh2Filename << "\")" << std::endl;
                        scriptFile << "var mesh2.sub(mesh2)" << std::endl;
                        scriptFile << "show(mesh1, color(0.8, 0.0, 0.7, 0.5))" << std::endl;
                    }
                    #endif

                    break;
                }
            }
        }

        // Clean the mesh after last iteration
        work = currentNumberOfSlices != numberOfSlices;

        for (mesh<type>* m : meshes)
        {
            for (int i = m->triangles.size() - 1; i >= 0; i--)
            {
                if (m->triangles[i]->hasFlag(triangle<type>::invalid))
                {
                    m->removeTriangle(i);
                }
            }
        }

        numberOfIterations++;
    }

    log(info) << "Finish cutting after " << numberOfSlices << " cuts from " << numberOfIterations << " iterations";
}

template <typename type>
bool mesh<type>::cutTriangle(size_t index, const intersection<type>& intersection)
{
    const triangle<type>& cutTriangle = *triangles[index];
    size_t newIndex = addVertex(intersection.position);
    std::vector<size_t> newTriangles;
    newTriangles.reserve(3);

    switch (intersection.hint)
    {
    case intersectionHint::ab:
    case intersectionHint::bc:
    case intersectionHint::ac:
        newTriangles.push_back(addTriangle(cutTriangle[intersection.hint % 3], newIndex, cutTriangle[(2 + intersection.hint) % 3]));
        newTriangles.push_back(addTriangle(cutTriangle[(1 + intersection.hint) % 3], cutTriangle[(2 + intersection.hint) % 3], newIndex));
        break;

    case intersectionHint::inside:
        newTriangles.push_back(addTriangle(cutTriangle[0], cutTriangle[1], newIndex));
        newTriangles.push_back(addTriangle(cutTriangle[1], cutTriangle[2], newIndex));
        newTriangles.push_back(addTriangle(cutTriangle[2], cutTriangle[0], newIndex));
        break;

    default:
        log(err) << "bad intersection hint";
        break;
    }

    triangles[index]->setFlag(triangle<type>::invalid, true);

    for (size_t index : newTriangles)
    {
        if (index >= triangles.size())
        {
            return false;
        }
    }
    return !newTriangles.empty();
}

template <typename type>
bool mesh<type>::isInside(const vector3<type>& point, bool onSideEqualInside)
{
    ray<type> ray(point, vector3<type>(10, 0, 0)); // todo bounds max
    type intersection(0);

    for (const auto* triangle : triangles)
    {
        // compute intersections
        type t, u, v;
        auto result = triangleRayIntersection<type>::intersects(ray, vertices[(*triangle)[0]], vertices[(*triangle)[1]], vertices[(*triangle)[2]], t, u, v);

        if (result == triangleRayIntersection<type>::inside)
        {
            if (t <= ratioEpsilon<type>())
            {
                if (onSideEqualInside)
                {
                    return true;
                }
                else
                {
                    continue;
                }
            }

            // handle intersection on vertex
            bool uEquals0 = equals<type>(u, type(0), ratioEpsilon<type>());
            bool uEquals1 = equals<type>(u, type(1), ratioEpsilon<type>());
            bool vEquals0 = equals<type>(v, type(0), ratioEpsilon<type>());
            bool vEquals1 = equals<type>(v, type(1), ratioEpsilon<type>());

            bool isA = uEquals0 and vEquals0;
            bool isB = uEquals1 and vEquals0;
            bool isC = uEquals0 and vEquals1;

            if (isA or isB or isC)
            {
                log(err) << "intersection on vertex unimplemented";
            }
            else
            {
                bool entering = triangle->getNormal().dot(ray.getDirection()) > type(0);
                intersection += entering ? type(1) : type(-1);
            }
        }
    }

    return int(intersection) < 0;
}

template <typename type>
void mesh<type>::mergeMesh(booleanOperation operation, mesh<type>* other)
{
    std::vector<std::array<vector3<type>, 3>> mergedTriangles;
    mergedTriangles.reserve(triangles.size() + other->triangles.size());

    for (const auto& triangle : triangles)
    {
        // Check if centroid is inside
        const auto& a = vertices[(*triangle)[0]];
        const auto& b = vertices[(*triangle)[1]];
        const auto& c = vertices[(*triangle)[2]];
        vector3<type> center((a + b + c) / type(3));
        bool inside = other->isInside(center, true);

        // Resolve
        if (((operation == booleanOperation::addition or operation == booleanOperation::difference) and !inside) or (operation == booleanOperation::intersection and inside))
        {
            mergedTriangles.push_back({ a, b, c });
        }
    }

    for (const auto& triangle : other->triangles)
    {
        // Check if centroid is inside
        const auto& a = other->vertices[(*triangle)[0]];
        const auto& b = other->vertices[(*triangle)[1]];
        const auto& c = other->vertices[(*triangle)[2]];
        vector3<type> center((a + b + c) / type(3));
        bool inside = isInside(center, false);

        // Resolve
        if ((operation == booleanOperation::addition and !inside) or (operation == booleanOperation::intersection and inside))
        {
            mergedTriangles.push_back({ a, b, c });
        }
        else if (operation == booleanOperation::difference and inside)
        {
            mergedTriangles.push_back({ c, b, a });
        }
    }

    clear();

    for (const auto& triangle : mergedTriangles)
    {
        addTriangle(triangle[0], triangle[1], triangle[2]);
    }

}

template <typename type>
void mesh<type>::refine()
{

}
