#include <algorithm>
#include "camera.h"

using namespace snail;

const float camera::m_sensitivity = 0.01f;

camera::camera()
	: m_distance(3.f) , m_rotation(0.f, 0.f)
{
	update_matrix();
}

void camera::zoom(float zoom)
{
	m_distance = std::max(m_distance - zoom, 0.1f);
	update_matrix();
}

void camera::move(double x, double y)
{
	m_rotation[0] -= static_cast<float>(x * m_sensitivity);
    const float max_rotation = pi2<float>() - 0.001f;
	m_rotation[1] = std::clamp(m_rotation[1] + static_cast<float>(y) * m_sensitivity, -max_rotation, max_rotation);
	update_matrix();
}

void camera::update_matrix()
{
    glm::mat4 transformation = glm::eulerAngleXY(-m_rotation[1], -m_rotation[0]);
    m_position = glm::vec4(0., 0., -m_distance, 1.f) * transformation;

    m_projection = glm::perspective(deg2rad(45.f), 800.f / 600.f, 0.01f, 100.f);
    m_view = glm::lookAt(m_position, glm::vec3(0.0f, 0.0f, 0.f), glm::vec3(0.0f, 1.f, 0.f));
}

void camera::set_distance(float distance)
{
	m_distance = distance;
}

const glm::mat4& camera::view() const
{
	return m_view;
}

const glm::mat4& camera::projection() const
{
	return m_projection;
}

const glm::vec3& camera::get_position() const
{
    return m_position;
}
