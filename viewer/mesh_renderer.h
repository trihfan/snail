/*
* Default mesh renderer
*/
#include <memory>
#include "glad/glad.h"
#include "color.h"
#pragma once

class Shader;

namespace snail
{
    template <typename Type>
    class mesh;

    class mesh_renderer
    {
    public:
        enum renderType { vertex, edges, triangles };

         /*
         * Constructor
         */
         mesh_renderer(mesh<double>& mesh, const color& color);

         /*
         * Render the mesh
         */
         void render(renderType type) const;

         color m_color;

    private:
        /*
        * Vertex buffer
        */
        GLuint m_vertex_buffer;

        /*
        * Vertex array
        */
        GLuint m_vertex_array;

        /*
        * Element buffer for the faces
        */
        GLuint m_element_buffer_faces;

        /*
        * Element buffer for the edges
        */
        GLuint m_element_buffer_edges;

        GLuint m_element_buffer_vertices;

        /*
        * The number of indices to draw for the faces
        */
        unsigned int m_number_of_indices_faces;

        /*
        * The number of indices to draw for the edges
        */
        unsigned int m_number_of_indices_edges;

        unsigned int m_number_of_indices_vertices;
    };
}
