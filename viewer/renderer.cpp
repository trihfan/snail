#include "renderer.h"
#include "shader.h"
#include "camera.h"

using namespace snail;

static const std::array<GLfloat, 8> m_quad_vertices
{
    -1.0f, -1.0f,
    1.0f, -1.0f,
    -1.0f,  1.0f,
    1.0f,  1.0f,
};

void renderer::init(int width, int height)
{
    if (m_first_init)
    {
        m_first_init = false;
        create(width, height);
    }
    initPrivate(width, height);
}

void renderer::create(int, int)
{
    // Load shader
    m_render_shader = std::make_unique<Shader>("shaders/depth_peeling/pass_render.vs.glsl", "shaders/depth_peeling/pass_render.fs.glsl");
    m_blend_shader = std::make_unique<Shader>("shaders/depth_peeling/blend.vs.glsl", "shaders/depth_peeling/blend.fs.glsl");
    m_fxaa_shader = std::make_unique<Shader>("shaders/fxaa.vs.glsl", "shaders/fxaa.fs.glsl");
}

static void init_texture(int width, int height, GLuint& tex, GLuint& dtex, GLuint& fbo)
{
    const auto & gen_tex = [](GLuint & tex)
    {
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    };

    // Generate texture for colors and attached to color component of framebuffer
    gen_tex(tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_BGRA, GL_FLOAT, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    // Generate texture for depth and attached to depth component of framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);
    gen_tex(dtex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, dtex, 0);

    // Clean up
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D,0);
}

void renderer::initPrivate(int width, int height)
{
    m_width = width;
    m_height = height;

    for (int i = 0; i < m_number_of_passes; i++)
    {
        init_texture(m_width, m_height, passFramebuffers[i].colorBuffer, passFramebuffers[i].depthBuffer, passFramebuffers[i].framebuffer);
    }

    init_texture(m_width, m_height, fxaaFramebuffer.colorBuffer, fxaaFramebuffer.depthBuffer, fxaaFramebuffer.framebuffer);

    // square
    glGenVertexArrays(1, &m_quad_vao);
    glBindVertexArray(m_quad_vao);
    glGenBuffers(1, &m_quad_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_quad_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m_quad_vertices.size(), m_quad_vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);
}

void renderer::render(camera& camera)
{
    // Setup state
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.f, 1.f, 1.f, 1.f);
    glLineWidth(3.f);
    glPointSize(6.f);

    // Render passes
    m_render_shader->use();
    glm::mat4 matrix = camera.projection() * camera.view();
    m_render_shader->setMatrix4("view", glm::value_ptr(matrix));
    m_render_shader->setFloat("width", m_width);
    m_render_shader->setFloat("height", m_height);

    glDisable(GL_BLEND);

    for (int i = 0; i < m_number_of_passes; i++)
    {
        m_render_shader->setInt("firstPass", i == 0);

        if (i > 0)
        {
            glActiveTexture(GL_TEXTURE0 + 0);
            glBindTexture(GL_TEXTURE_2D, passFramebuffers[i-1].depthBuffer);
        }

        glBindFramebuffer(GL_FRAMEBUFFER, passFramebuffers[i].framebuffer);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_render_shader->setInt("depthTexture", 0);
        for (const mesh_renderer& renderer : m_meshes)
        {
            m_render_shader->setVec4("inputColor", color(0.f, 0.f, 0.f, 1.f).data());
            //renderer.render(mesh_renderer::vertex);

            m_render_shader->setVec4("inputColor", color(0.f, 0.f, 0.f, 1.f).data());
            renderer.render(mesh_renderer::edges);

            m_render_shader->setVec4("inputColor", renderer.m_color.data());
            renderer.render(mesh_renderer::triangles);
        }
    }

    //
    glBindFramebuffer(GL_FRAMEBUFFER, fxaaFramebuffer.framebuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render final result as composite of all textures
    m_blend_shader->use();
    glBindVertexArray(m_quad_vao);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glDepthFunc(GL_ALWAYS);

    for(int pass = m_number_of_passes-1;pass>=0;pass--)
    {
        m_blend_shader->setInt("colorTexture", 0);
        glActiveTexture(GL_TEXTURE0 + 0);
        glBindTexture(GL_TEXTURE_2D, passFramebuffers[pass].colorBuffer);

        m_blend_shader->setInt("depthTexture", 1);
        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_2D, passFramebuffers[pass].depthBuffer);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);       

    }

    glDepthFunc(GL_LESS);



    // Fxaa pass
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, fxaaFramebuffer.colorBuffer);

    m_fxaa_shader->use();


    m_fxaa_shader->setInt("u_colorTexture", 0);
    m_fxaa_shader->setVec2("u_texelStep", glm::value_ptr(glm::vec2(1. / m_width, 1. / m_height)));

    m_fxaa_shader->setInt("u_fxaaOn", 1);
    m_fxaa_shader->setInt("u_showEdges", 0);

    m_fxaa_shader->setFloat("u_lumaThreshold", 0.1f);
    m_fxaa_shader->setFloat("u_mulReduce", 1.0f / 8.f);
    m_fxaa_shader->setFloat("u_minReduce", 1.0f / 128.f);
    m_fxaa_shader->setFloat("u_maxSpan", 8.f);

    glBindVertexArray(m_quad_vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void renderer::add_mesh(mesh<double>& mesh, const color& color)
{
    m_meshes.push_back(mesh_renderer(mesh, color));
}
