/*
* An orbit camera
*/
#include "snail/snail.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/hash.hpp>

#pragma once

struct GLFWwindow;

namespace snail
{
	class camera
	{
	public:
		/*
		* Constructor
		*/
		camera();

		/*
		* Zoom the camera
		* @param zoom The zoom movement
		*/
		void zoom(float zoom);

		/*
		* Move the camera
		* @param x The x movement of the mouse
		* @param y The y movement of the mouse
		*/
		void move(double x, double y);

		/*
		* Set the camera distance
		*/
		void set_distance(float distance);

		/*
		* Return the camera view matrix
		*/
        const glm::mat4& view() const;

		/*
		* Return the camera projection matrix
		*/
        const glm::mat4& projection() const;

		/*
		* Return the camera position
		*/
        const glm::vec3& get_position() const;

	private:
		/*
		* The camera sensitivity
		*/
		static const float m_sensitivity;

		/*
		* The camera distance
		*/
		float m_distance;

		/*
		* The camera rotation
		*/
        glm::vec2 m_rotation;

		/*
		* The camera position
		*/
        glm::vec3 m_position;

		/*
		* The view matrix
		*/
        glm::mat4 m_view;

		/*
		* The projection matrix
		*/
        glm::mat4 m_projection;

		/*
		* Update the camera matrix
		*/
		void update_matrix();
	};
}
