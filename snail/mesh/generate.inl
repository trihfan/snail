template <typename type>
mesh<type>* generate::box(const vector3<type>& position, const vector3<type>& size)
{
    vector3<type> halfSize = size / type(2);

    // vertices
    auto box = new mesh<type>(8);
    box->addVertex(position + vector3<type>(-halfSize.x(), -halfSize.y(), -halfSize.z()));
    box->addVertex(position + vector3<type>( halfSize.x(), -halfSize.y(), -halfSize.z()));
    box->addVertex(position + vector3<type>( halfSize.x(),  halfSize.y(), -halfSize.z()));
    box->addVertex(position + vector3<type>(-halfSize.x(),  halfSize.y(), -halfSize.z()));
    box->addVertex(position + vector3<type>( halfSize.x(),  halfSize.y(),  halfSize.z()));
    box->addVertex(position + vector3<type>(-halfSize.x(),  halfSize.y(),  halfSize.z()));
    box->addVertex(position + vector3<type>( halfSize.x(), -halfSize.y(),  halfSize.z()));
    box->addVertex(position + vector3<type>(-halfSize.x(), -halfSize.y(),  halfSize.z()));

    // indices
    box->addTriangle(2, 1, 0);
    box->addTriangle(3, 2, 0);
    box->addTriangle(5, 2, 3);
    box->addTriangle(5, 4, 2);
    box->addTriangle(2, 6, 1);
    box->addTriangle(4, 6, 2);
    box->addTriangle(7, 6, 5);
    box->addTriangle(6, 4, 5);
    box->addTriangle(7, 5, 3);
    box->addTriangle(3, 0, 7);
    box->addTriangle(0, 1, 7);
    box->addTriangle(1, 6, 7);

    return box;
}

template <typename type>
mesh<type>* generate::sphere(const vector3<type>& position, type radius, size_t u, size_t v)
{
    auto sphere = new mesh<type>(u * v);

    type sectorStep = 2 * pi<type>() / v;
    type stackStep = pi<type>() / u;
    type sectorAngle, stackAngle;

    for (int i = 0; i <= u; ++i)
    {
        stackAngle = pi<type>() / 2 - i * stackStep;     // starting from pi/2 to -pi/2
        type xy = radius * cosf(stackAngle);             // r * cos(u)
        type z = radius * sinf(stackAngle);              // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for (int j = 0; j <= v; ++j)
        {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            // vertex position (x, y, z)
            type x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            type y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)

            sphere->addVertex(position + vector3<type>(type(x), type(y), type(z)));
        }
    }


    // generate CCW index list of sphere triangles
    std::vector<int> indices;
    int k1, k2;
    for (int i = 0; i < u; ++i)
    {
        k1 = i * (v + 1);     // beginning of current stack
        k2 = k1 + v + 1;      // beginning of next stack

        for (int j = 0; j < v; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding first and last stacks
            // k1 => k2 => k1+1
            if (i != 0)
            {
                sphere->addTriangle(k1, k2, k1 + 1);
            }

            // k1+1 => k2 => k2+1
            if (i != (u-1))
            {
                sphere->addTriangle(k1 + 1, k2, k2 + 1);
            }
        }
    }

    return sphere;
}

template <typename type>
mesh<type>* generate::pyramid(const vector3<type>& position, type height, type length)
{
    type halfLength = length / type(2);
    type halfHeight = height / type(2);

    // vertices
    auto pyramid = new mesh<type>(5);
    pyramid->addVertex(position + vector3<type>(-halfLength, -halfLength, -halfHeight));
    pyramid->addVertex(position + vector3<type>( halfLength, -halfLength, -halfHeight));
    pyramid->addVertex(position + vector3<type>( halfLength,  halfLength, -halfHeight));
    pyramid->addVertex(position + vector3<type>(-halfLength,  halfLength, -halfHeight));
    pyramid->addVertex(position + vector3<type>(0, 0, halfHeight));

    // indices
    pyramid->addTriangle(0, 1, 3);
    pyramid->addTriangle(1, 2, 3);
    pyramid->addTriangle(0, 1, 4);
    pyramid->addTriangle(1, 2, 4);
    pyramid->addTriangle(2, 3, 4);
    pyramid->addTriangle(3, 0, 4);

    return pyramid;
}

template <typename type>
mesh<type>* generate::tetrahedron(const vector3<type>& position, type height, type length)
{
    auto tetrahedron = new mesh<type>(4);

    return tetrahedron;
}

template <typename type>
mesh<type>* generate::load(const std::string& filename)
{
    std::ifstream input(filename);
    nlohmann::json json;
    input >> json;
    return json.get<mesh<type>*>();
}
