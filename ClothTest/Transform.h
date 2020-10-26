#pragma once
#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"
#include "glm/gtc/quaternion.hpp"

class Transform
{
public:
	Transform();
	Transform(const glm::vec3& a_pos, const glm::quat& a_rotation, const glm::vec3& a_scale);

	inline const glm::mat4x4 getMatrix()const;
	inline const glm::mat4x4 getInverse()const;

	void translate(const glm::vec3& a_toMove);
	void rotateAroundAxis(const glm::vec3& a_axis, float a_toRotate);
	inline void rotateAroundX(float a_toRotate);
	inline void rotateAroundY(float a_toRotate);
	inline void rotateAroundZ(float a_toRotate);
	void slerpRotation(const glm::quat& a_target, float a_delta);
	void lerpRotation(const glm::quat& a_target, float a_delta);
	void rotateTowardsWorldPos(const glm::vec3& a_target);
	void lerpRotationTowardsWorldPos(const glm::vec3& a_target, float a_delta);
	void slerpRotationTowardsWorldPos(const glm::vec3& a_target, float a_delta);
	void scale(const glm::vec3& a_toScale);

	inline void setPos(const glm::vec3& a_newPos);
	inline void setRotation(const glm::quat& a_newRotation);
	inline void setScale(const glm::vec3& a_newScale);

	inline glm::vec3 getRight()const;
	inline glm::vec3 getUp()const;
	inline glm::vec3 getForward()const;

	inline const glm::vec3 getPos()const;
	inline const glm::quat getRotation()const;
	inline const glm::vec3 getScale()const;

	inline glm::vec3 toWorldPos(const glm::vec3& a_localPos)const;
	inline glm::vec3 toLocalPos(const glm::vec3& a_worldPos)const;
	inline glm::vec3 toWorldDir(const glm::vec3& a_localDir)const;
	inline glm::vec3 toLocalDir(const glm::vec3& a_worldDir)const;

	
private:
	//local matrices
	glm::mat4x4 m_matrix;
	glm::mat4x4 m_inverse;

	//separated local transformation data
	glm::vec3 m_pos;
	glm::quat m_rotation;
	glm::vec3 m_scale;

	//updates matrices
	void updateMatrices();
	
};

//include templated/inline function
#include "Transform.inl"