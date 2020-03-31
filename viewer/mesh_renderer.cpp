#include "mesh_renderer.h"
#include "snail/snail.h"
#include "color.h"
#include <numeric>

using namespace snail;

mesh_renderer::mesh_renderer(mesh<double>& mesh, const color& color)
    : m_color(color)
{
    // Vertices
    std::vector<float> vertices(mesh.getVertexCount() * 3);
    for (size_t i = 0; i < mesh.getVertexCount(); i++)
    {
        vertices[i * 3] = mesh.getVertex(i).x();
        vertices[i * 3 + 1] = mesh.getVertex(i).y();
        vertices[i * 3 + 2] = mesh.getVertex(i).z();
    }

    std::vector<unsigned int> indices_vertices(mesh.getVertexCount());
    std::iota(indices_vertices.begin(), indices_vertices.end(), 0);
    m_number_of_indices_vertices = static_cast<unsigned int>(mesh.getVertexCount());

    // Faces
    m_number_of_indices_faces = static_cast<unsigned int>(mesh.getTriangleCount() * 3);
    std::vector<unsigned int> indices_faces(m_number_of_indices_faces);
    for (size_t i = 0; i < mesh.getTriangleCount(); i++)
    {
        const auto& triangle = mesh.getTriangle(i);
        indices_faces[i * 3] = triangle[0];
        indices_faces[i * 3 + 1] = triangle[1];
        indices_faces[i * 3 + 2] = triangle[02];
    }

    // Edges
    std::vector<unsigned int> indices_edges(m_number_of_indices_faces * 2);
    for (size_t i = 0; i < mesh.getTriangleCount(); i++)
    {
        const auto& triangle = mesh.getTriangle(i);
        unsigned int index_0 = triangle[0];
        unsigned int index_1 = triangle[1];
        unsigned int index_2 = triangle[2];

        indices_edges[i * 6] = index_0;
        indices_edges[i * 6 + 1] = index_1;
        indices_edges[i * 6 + 2] = index_1;
        indices_edges[i * 6 + 3] = index_2;
        indices_edges[i * 6 + 4] = index_2;
        indices_edges[i * 6 + 5] = index_0;
    }

    m_number_of_indices_edges = static_cast<unsigned int>(indices_edges.size());

    // Generate buffers
    glGenVertexArrays(1, &m_vertex_array);
    glGenBuffers(1, &m_vertex_buffer);
    glGenBuffers(1, &m_element_buffer_vertices);
    glGenBuffers(1, &m_element_buffer_faces);
    glGenBuffers(1, &m_element_buffer_edges);

    // Bind vertex array
    glBindVertexArray(m_vertex_array);

    // Copy vertices
    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    // Copy index array vertices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_element_buffer_vertices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices_vertices.size(), indices_vertices.data(), GL_STATIC_DRAW);

    // Copy index array faces
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_element_buffer_faces);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices_faces.size(), indices_faces.data(), GL_STATIC_DRAW);

    // Copy index array edges
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_element_buffer_edges);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices_edges.size(), indices_edges.data(), GL_STATIC_DRAW);

    // Set vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}


void mesh_renderer::render(renderType type) const
{
    glBindVertexArray(m_vertex_array);

    switch (type)
    {
    case vertex:
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_element_buffer_vertices);
        glDrawElements(GL_POINTS, m_number_of_indices_vertices, GL_UNSIGNED_INT, 0);
        break;

    case edges:
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_element_buffer_edges);
        glDrawElements(GL_LINES, m_number_of_indices_edges, GL_UNSIGNED_INT, 0);
        break;

    case triangles:
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_element_buffer_faces);
        glDrawElements(GL_TRIANGLES, m_number_of_indices_faces, GL_UNSIGNED_INT, 0);
        break;
    }
}
