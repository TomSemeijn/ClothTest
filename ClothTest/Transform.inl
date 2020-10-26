#pragma once
#include "Transform.h"//for intellisense - cancelled out by pragma once

inline void Transform::rotateAroundX(float a_toRotate)
{
	rotateAroundAxis(glm::vec3(1.f, 0.f, 0.f), a_toRotate);
}

inline void Transform::rotateAroundY(float a_toRotate)
{
	rotateAroundAxis(glm::vec3(0.f, 1.f, 0.f), a_toRotate);
}

inline void Transform::rotateAroundZ(float a_toRotate)
{
	rotateAroundAxis(glm::vec3(0.f, 0.f, 1.f), a_toRotate);
}

inline glm::vec3 Transform::getRight()const
{
	return glm::normalize(glm::vec3(getMatrix()[0]));
}

inline glm::vec3 Transform::getUp()const
{
	return glm::normalize(glm::vec3(getMatrix()[1]));
}

inline glm::vec3 Transform::getForward()const
{
	return glm::normalize(glm::vec3(getMatrix()[2]));
}

inline void Transform::setPos(const glm::vec3& a_newPos)
{
	m_pos = a_newPos;
	updateMatrices(); //TODO: make this not update every time
}

inline void Transform::setRotation(const glm::quat& a_newRotation)
{
	m_rotation = a_newRotation;
	updateMatrices(); //TODO: make this not update every time
}

inline void Transform::setScale(const glm::vec3& a_newScale)
{
	m_scale = a_newScale;
	updateMatrices(); //TODO: make this not update every time
}

inline glm::vec3 Transform::toWorldPos(const glm::vec3& a_localPos)const
{
	return getMatrix() * glm::vec4(a_localPos, 1.f);
}

inline glm::vec3 Transform::toLocalPos(const glm::vec3& a_worldPos)const
{
	return getInverse() * glm::vec4(a_worldPos, 1.f);
}

inline glm::vec3 Transform::toWorldDir(const glm::vec3& a_localDir)const
{
	return getMatrix() * glm::vec4(a_localDir, 0.f);
}

inline glm::vec3 Transform::toLocalDir(const glm::vec3& a_worldDir)const
{
	return getInverse() * glm::vec4(a_worldDir, 0.f);
}

inline const glm::vec3 Transform::getPos()const
{
	return m_pos;
}

inline const glm::quat Transform::getRotation()const
{
	return m_rotation;
}

inline const glm::vec3 Transform::getScale()const
{
	return m_scale;
}

inline const glm::mat4x4 Transform::getMatrix()const
{
	return m_matrix;
}

inline const glm::mat4x4 Transform::getInverse()const
{
	return m_inverse;
}
