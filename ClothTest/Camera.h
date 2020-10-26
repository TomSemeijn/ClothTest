#pragma once
#include <glm/mat4x4.hpp>

class Camera
{
public:
	Camera(float a_fov, const glm::uvec2& a_windowSize, float a_near, float a_far, const glm::vec3& a_position, const glm::vec3& a_viewDirection, const glm::vec3& a_up);

	const glm::mat4& getView()const;
	const glm::mat4& getProjection()const;

private:
	glm::mat4 m_view;
	glm::mat4 m_projection;

};
