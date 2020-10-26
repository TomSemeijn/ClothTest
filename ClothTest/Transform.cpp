#include "Transform.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"

Transform::Transform()
	:	m_matrix(glm::identity<glm::mat4x4>()),
		m_inverse(m_matrix),
		m_pos(0.f, 0.f, 0.f),
		m_rotation(glm::identity<glm::quat>()),
		m_scale(1.f, 1.f, 1.f)
{}

Transform::Transform(const glm::vec3& a_pos, const glm::quat& a_rotation, const glm::vec3& a_scale)
	:	m_matrix(glm::identity<glm::mat4x4>()),
		m_pos(a_pos),
		m_rotation(a_rotation),
		m_scale(a_scale)
{
	updateMatrices();
}

void Transform::translate(const glm::vec3& a_toMove)
{
	m_pos += a_toMove;
	updateMatrices();
}

void Transform::rotateAroundAxis(const glm::vec3& a_axis, float a_toRotate)
{
	m_rotation = glm::rotate(m_rotation, a_toRotate, a_axis);
	updateMatrices();
}

void Transform::slerpRotation(const glm::quat& a_target, float a_delta)
{
	m_rotation = glm::slerp(m_rotation, a_target, a_delta);
	updateMatrices();
}

void Transform::lerpRotation(const glm::quat& a_target, float a_delta)
{
	m_rotation = glm::mix(m_rotation, a_target, a_delta);
	updateMatrices();
}

void Transform::rotateTowardsWorldPos(const glm::vec3& a_target)
{
	//create rotation matrix using new axes
	const auto& pos = getPos();
	const glm::mat4x4 lookAtMatrix = glm::inverse(glm::lookAt(pos, pos - (a_target - pos), getUp()));
	const glm::mat4x4 newRotationMatrix{
		glm::vec4(glm::normalize(glm::vec3(lookAtMatrix[0])), 0.f),
		glm::vec4(glm::normalize(glm::vec3(lookAtMatrix[1])), 0.f),
		glm::vec4(glm::normalize(glm::vec3(lookAtMatrix[2])), 0.f),
		glm::vec4(0.f, 0.f, 0.f, 1.f)
	};

	//create new quaternion from rotation matrix
	m_rotation = glm::quat_cast(newRotationMatrix);

	updateMatrices();

}

void Transform::lerpRotationTowardsWorldPos(const glm::vec3& a_target, float a_delta)
{
	const auto& pos = getPos();
	const glm::mat4x4 lookAtMatrix = glm::inverse(glm::lookAt(pos, pos - (a_target - pos), getUp()));
	const glm::mat4x4 newRotationMatrix{
		glm::vec4(glm::normalize(glm::vec3(lookAtMatrix[0])), 0.f),
		glm::vec4(glm::normalize(glm::vec3(lookAtMatrix[1])), 0.f),
		glm::vec4(glm::normalize(glm::vec3(lookAtMatrix[2])), 0.f),
		glm::vec4(0.f, 0.f, 0.f, 1.f)
	};
	lerpRotation(glm::quat_cast(newRotationMatrix), a_delta);
}

void Transform::slerpRotationTowardsWorldPos(const glm::vec3& a_target, float a_delta)
{
	const auto& pos = getPos();
	const glm::mat4x4 lookAtMatrix = glm::inverse(glm::lookAt(pos, pos - (a_target - pos), getUp()));
	const glm::mat4x4 newRotationMatrix{
		glm::vec4(glm::normalize(glm::vec3(lookAtMatrix[0])), 0.f),
		glm::vec4(glm::normalize(glm::vec3(lookAtMatrix[1])), 0.f),
		glm::vec4(glm::normalize(glm::vec3(lookAtMatrix[2])), 0.f),
		glm::vec4(0.f, 0.f, 0.f, 1.f)
	};
	slerpRotation(glm::quat_cast(newRotationMatrix), a_delta);
}

void Transform::scale(const glm::vec3& a_toScale)
{
	m_scale *= a_toScale;
	updateMatrices();
}

//updates matrices
//TODO: make more efficient, potentially an isDirty boolean
void Transform::updateMatrices()
{
	m_matrix = glm::translate(glm::identity<glm::mat4x4>(), m_pos);
	m_matrix = m_matrix * mat4_cast(m_rotation);
	m_matrix = glm::scale(m_matrix, m_scale);
	m_inverse = glm::inverse(m_matrix);
}
