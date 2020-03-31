/*
* R
*/

#include <vector>
#include "mesh_renderer.h"
#pragma once

namespace snail
{
    class camera;

    class renderer
	{
	public:
		/*
		* Constructor
		*/
        renderer() = default;

        /*
        * Destructor
        */
        virtual ~renderer() = default;

        /*
        * Initialize the renderer
        */
        void init(int width, int height);

        /*
        * Ask the renderer the render the items
        */
        void render(camera& camera);

		/*
		* Add a mesh to the app
		* @param mesh The mesh
		* @param color The mesh rendering color
		*/
        void add_mesh(mesh<double>& mesh, const color& color);

    private:
        /*
        * Create the renderer
        */
        void create(int width, int height);

        /*
        * Init the renderer
        */
        void initPrivate(int width, int height);

        /*
        * Flag to know if we have to initialize everythings
        */
        bool m_first_init = true;

        struct FrameBuffer
        {
            GLuint colorBuffer;
            GLuint depthBuffer;
            GLuint framebuffer;
        };

        /*
        * Number of passes
        */
        static constexpr int m_number_of_passes = 4;

        /*
        * Mesh list
        */
        std::vector<mesh_renderer> m_meshes;

        /*
        * Shader used during render the passes
        */
        std::unique_ptr<Shader> m_render_shader;

        /*
        * Shader used to blend the passes results
        */
        std::unique_ptr<Shader> m_blend_shader;

        std::unique_ptr<Shader> m_fxaa_shader;

        /*
        * Screen size
        */
        float m_width, m_height;

        /*
        *
        */
        FrameBuffer passFramebuffers[m_number_of_passes];

        FrameBuffer fxaaFramebuffer;

        /*
        *
        */
        GLuint m_quad_vbo;
        GLuint m_quad_vao;
	};
}
