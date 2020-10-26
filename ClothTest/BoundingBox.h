#pragma once
#include <glm/vec3.hpp>

struct BoundingBox
{
	glm::vec3 m_min, m_max;

	BoundingBox();
	BoundingBox(const glm::vec3& a_min, const glm::vec3& a_max);

	bool intersectsBoundingBox(const BoundingBox& a_other)const;
	bool intersectsSphere(const glm::vec3& a_pos, float a_radius)const;
	bool fitsEntirelyWithin(const BoundingBox& a_box)const;
	glm::vec3 getCenter()const;
};
