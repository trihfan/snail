template <typename type>
mesh<type>::mesh(size_t reserveSize)
    : id(sCurrentIndex++)
{
    vertices.reserve(reserveSize);
    triangles.reserve(reserveSize / 3);
}

template <typename type>
mesh<type>::~mesh()
{
    for (auto t : triangles)
    {
        delete t;
    }
}

template <typename type>
mesh_t<type> mesh<type>::clone() const
{
    auto clone = std::make_unique<mesh<type>>();

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
        log::err << "add triangle with not 3 separates indices";
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
void mesh<type>::add(mesh_t<type> mesh)
{
    compute(booleanOperation::addition, std::move(mesh));
}

template <typename type>
void mesh<type>::sub(mesh_t<type> mesh)
{
    compute(booleanOperation::difference, std::move(mesh));
}

template <typename type>
void mesh<type>::inter(mesh_t<type> mesh)
{
    compute(booleanOperation::intersection, std::move(mesh));
}

template <typename type>
void mesh<type>::compute(booleanOperation operation, mesh_t<type> other)
{
    // cut triangles
    cutMesh(other.get());

    // merge
    mergeMesh(operation, other.get());

    // refine
    //refine();
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
                    log::info << "START CUT";
                    log::info << "triangle 1: " << *current;
                    log::info << "triangle 2: " << *otherTriangle;

                    bool cut1 = meshes[meshAIndex]->cutTriangle(current->getIndex(), intersectionA);
                    bool cut2 = meshes[meshBIndex]->cutTriangle(otherTriangle->getIndex(), intersectionB);

                    log::info << "END CUT" << std::endl;

                    if (cut1 or cut2)
                    {
                        numberOfSlices += cut1 + cut2;
                        break;
                    }
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

        //if (numberOfIterations >= 3) break;
    }

    log::info << "Finish cutting after " << numberOfSlices << " cuts from " << numberOfIterations << " iterations";
}

template <typename type>
bool mesh<type>::cutTriangle(size_t index, const std::vector<intersection<type>>& intersections)
{
    // Cut the triangle
    switch (intersections.size())
    {
    case 1:
        cut1<type>::cut(this, intersections, *triangles[index]);
        triangles[index]->setFlag(triangle<type>::invalid, true);
        return true;

    case 2:
        cut2<type>::cut(this, intersections, *triangles[index]);
        triangles[index]->setFlag(triangle<type>::invalid, true);
        return true;

    default:
        if (!intersections.empty())
        {
            log::err << "Bad classification, " << intersections.size() << " intersections";
            cut1<type>::cut(this, intersections, *triangles[index]);
            triangles[index]->setFlag(triangle<type>::invalid, true);
            return true;
        }
        return false;
    }
}

template <typename type>
void mesh<type>::mergeMesh(booleanOperation operation, mesh<type>* other)
{
    auto oldVertices = std::move(vertices);
    auto oldTriangles = std::move(triangles);
    verticesMap.clear();

    // other
    for (const auto* triangleA : other->triangles)
    {
        // Compute intersections
        const auto& a = other->vertices[(*triangleA)[0]];
        const auto& b = other->vertices[(*triangleA)[1]];
        const auto& c = other->vertices[(*triangleA)[2]];

        vector3<type> center((a + b + c) / type(3));
        ray<type> rayFromCenter(center, center + 1000 * triangleA->getSide(0).getDirection().cross(triangleA->getSide(1).getDirection()));
        std::vector<vector3<type>> intersections;

        for (const auto* triangleB : oldTriangles)
        {
            type t, u, v;
            auto result = triangleRayIntersection<type>::intersects(rayFromCenter, oldVertices[(*triangleB)[0]], oldVertices[(*triangleB)[1]], oldVertices[(*triangleB)[2]], t, u, v);
            vector3<type> position = rayFromCenter.getOrigin() + rayFromCenter.getDirection() * type(t);

            if (result == triangleRayIntersection<type>::inside)
            {
                bool found = false;
                for (const auto& intersection : intersections)
                {
                    if (equalsV(intersection, position))
                    {
                        found = true;
                        break;
                    }
                }

                if (!found)
                {
                    intersections.push_back(position);
                }
            }
        }

        // Resolve
        bool inside = intersections.size() % 2 == 1;
        switch (operation)
        {
        case booleanOperation::addition:
            if (!inside)
            {
                addTriangle(addVertex(a), addVertex(b), addVertex(c));
            }
            break;

        case booleanOperation::difference:
            if (inside)
            {
                addTriangle(addVertex(c), addVertex(b), addVertex(a));
            }
            break;

        case booleanOperation::intersection:
            if (inside)
            {
                addTriangle(addVertex(a), addVertex(b), addVertex(c));
            }
            break;
        }
    }

    // this
    for (const auto& triangleA : oldTriangles)
    {
        // Compute intersections
        const auto& a = oldVertices[(*triangleA)[0]];
        const auto& b = oldVertices[(*triangleA)[1]];
        const auto& c = oldVertices[(*triangleA)[2]];

        vector3<type> center((a + b + c) / type(3));
        ray<type> rayFromCenter(center, center + 1000 * triangleA->getSide(0).getDirection().cross(triangleA->getSide(1).getDirection()));
        std::vector<vector3<type>> intersections;

        for (const auto& triangleB : other->triangles)
        {
            type t, u, v;
            auto result = triangleRayIntersection<type>::intersects(rayFromCenter, other->vertices[(*triangleB)[0]], other->vertices[(*triangleB)[1]], other->vertices[(*triangleB)[2]], t, u, v);
            vector3<type> position = rayFromCenter.getOrigin() + rayFromCenter.getDirection() * type(t);

            if (result == triangleRayIntersection<type>::inside)
            {
                if (std::find_if(intersections.begin(), intersections.end(), [&position](const vector3<type>& inter) { return equalsV(inter, position); }) == intersections.end())
                {
                    intersections.push_back(position);
                }
            }
        }

        // Resolve
        bool inside = intersections.size() % 2 == 1;
        switch (operation)
        {
        case booleanOperation::addition:
            if (!inside)
            {
                addTriangle(addVertex(a), addVertex(b), addVertex(c));
            }
            break;

        case booleanOperation::difference:
            if (!inside)
            {
                addTriangle(addVertex(c), addVertex(b), addVertex(a));
            }
            break;

        case booleanOperation::intersection:
            if (inside)
            {
                addTriangle(addVertex(a), addVertex(b), addVertex(c));
            }
            break;
        }
    }
}

template <typename type>
void mesh<type>::refine()
{

}
