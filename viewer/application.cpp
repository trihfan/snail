#include <iostream>
#include <chrono>
#include <numeric>
#include <thread>
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "application.h"
#include "shader.h"

using namespace snail;
using namespace std::chrono;

// Runtime variables
GLFWwindow* application::m_window;
renderer application::m_renderer;
camera application::m_camera;
GLint application::m_current_width = 800;
GLint application::m_current_height = 600;
float application::m_window_scale = 1.f;
double application::m_last_x = 0;
double application::m_last_y = 0;
bool application::m_clicked = false;
std::deque<int> application::m_fps;

application::application()
{
    // Init glfw
    glfwInit();

    // Create the window
    if (!create_window())
    {
        throw (std::runtime_error("Cannot open the window"));
    }


    // Setup window
    glfwSetWindowUserPointer(m_window, this);

    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow*, int width, int height)
    {
        glViewport(0, 0, width, height);
    });

    glfwSetCursorPosCallback(m_window, &application::mouse_callback);
    glfwSetScrollCallback(m_window, &application::scroll_callback);
    glfwSetMouseButtonCallback(m_window, &application::mouse_button_callback);
    glfwSetWindowSizeCallback(m_window, &application::framebuffer_size_callback);

    glfwMakeContextCurrent(m_window);

    // Load opengl methods
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        return;
    }
}

bool application::create_window()
{
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    m_window = glfwCreateWindow(m_current_width, m_current_height, "snail - viewer", nullptr, nullptr);
    if (!m_window)
    {
        log::err << "unable to create opengl context" << std::endl;
        return false;
    }

#ifdef __APPLE__
    float mock;
    glfwGetWindowContentScale(m_window, &m_window_scale, &mock);
#endif
    return true;
}

void application::add_mesh(mesh<double>& mesh, const color& color)
{
    m_renderer.add_mesh(mesh, color);
}

int application::run()
{
    m_renderer.init(static_cast<GLint>(m_current_width * m_window_scale), static_cast<GLint>(m_current_height * m_window_scale));
    glfwSwapInterval(0);
    auto last = steady_clock::now();

    // Main loop
    while (!glfwWindowShouldClose(m_window))
    {
        // Input
        if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(m_window, true);
        }

        m_renderer.render(m_camera);

        // Swap
        glfwSwapBuffers(m_window);
        glfwPollEvents();

        // Fps counter
        auto now = steady_clock::now();
        if (m_fps.size() > 20) m_fps.pop_front();
        m_fps.push_back(static_cast<int>(1e6 / duration_cast<microseconds>(now - last).count()));
        std::stringstream ss;
        ss << "snail [" << std::accumulate(m_fps.begin(), m_fps.end(), 0) / m_fps.size() << " fps]";
        glfwSetWindowTitle(m_window, ss.str().c_str());
        last = now;

        std::this_thread::sleep_for(milliseconds(1));
    }

    return 0;
}

void application::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    m_current_width = static_cast<GLint>(width * m_window_scale);
    m_current_height = static_cast<GLint>(height * m_window_scale);
    glViewport(0, 0, m_current_width, m_current_height);
    m_renderer.init(m_current_width, m_current_height);
}

void application::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (m_clicked)
    {
        m_camera.move(xpos - m_last_x, ypos - m_last_y);
    }

    m_last_x = xpos;
    m_last_y = ypos;
}

void application::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    m_camera.zoom(static_cast<float>(yoffset));
}

void application::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_1)
    {
        m_clicked = action == GLFW_PRESS;
    }
}
