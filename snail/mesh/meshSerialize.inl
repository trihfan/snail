#ifdef SNAIL_JSON
template <typename type>
inline void to_json(nlohmann::json& j, const mesh<type>& m)
{
    auto& vertices = j["vertices"];
    for (size_t i = 0; i < m.getVertexCount(); i++)
    {
        vertices[i] = m.getVertex(i);
    }

    auto& triangles = j["triangles"];
    for (size_t i = 0; i < m.getTriangleCount(); i++)
    {
        auto& triangle = triangles[i];
        triangle["a"] = m.getTriangle(i)[0];
        triangle["b"] = m.getTriangle(i)[1];
        triangle["c"] = m.getTriangle(i)[2];
    }
}

template <typename type>
inline void from_json(const nlohmann::json& j, mesh<type>& m)
{
    const auto& vertices = j.at("vertices");
    for (const auto& vertex : vertices)
    {
        m.addVertex(vertex);
    }

    const auto& triangles = j.at("triangles");
    for (const auto& triangle : triangles)
    {
        m.addTriangle(triangle.at("a"), triangle.at("b"), triangle.at("c"));
    }
}

template <typename type>
inline void to_json(nlohmann::json& j, const mesh<type>*& m)
{
    to_json(j, *m);
}

template <typename type>
inline void from_json(const nlohmann::json& j, mesh<type>*& m)
{
    m = new mesh<type>();
    from_json(j, *m);
}
#endif
