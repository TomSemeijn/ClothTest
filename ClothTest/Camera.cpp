#include "Camera.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

Camera::Camera(float a_fov, const glm::uvec2& a_windowSize, float a_near, float a_far, const glm::vec3& a_position, const glm::vec3& a_viewDirection, const glm::vec3& a_up)
	: m_view(glm::perspectiveFovLH(glm::radians(a_fov), static_cast<float>(a_windowSize.x), static_cast<float>(a_windowSize.y), a_near, a_far))
	, m_projection(glm::lookAtLH(a_position, a_position + a_viewDirection, a_up))
{}

const glm::mat4& Camera::getView()const
{
	return m_view;
}

const glm::mat4& Camera::getProjection()const
{
	return m_projection;
}
