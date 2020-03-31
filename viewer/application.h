/*
* Application using glfw to display a snail mesh
*/

#include <deque>
#include "camera.h"
#include "renderer.h"
#pragma once

struct GLFWwindow;
class Shader;

namespace snail
{
	class application
	{
	public:
		/*
		* Constructor
		*/
		application();

        /*
        * Destructor
        */
        ~application() = default;

		/*
		* Add a mesh to the app
		* @param mesh The mesh
		* @param color The mesh rendering color
		*/
        void add_mesh(mesh<double>& mesh, const color& color);

		/*
		* Run the application
		*/
        int run();

	private:
        /*
        * The camera
        */
        static camera m_camera;

        /*
        * The current window width
        */
        static int m_current_width;

        /*
        * The current window height
        */
        static int m_current_height;

        static float m_window_scale;

        /*
        * The last mouse x position
        */
        static double m_last_x;

        /*
        * The last mouse y position
        */
        static double m_last_y;

        /*
        * True if the mous eis clicked
        */
        static bool m_clicked;

        /*
        * The window
        */
        static GLFWwindow* m_window;

		/*
        * The mesh renderer
		*/
        static renderer m_renderer;

        static std::deque<int> m_fps;

		/*
		* Resize callback
		*/
		static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

		/*
		* mouse callback
		*/
		static void mouse_callback(GLFWwindow* window, double xpos, double ypos);

		/*
		* Scroll callback
		*/
		static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

		/*
		* Mouse clic callback
		*/
        static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

        bool create_window();
	};
}
