#include "BoundingBox.h"
#include <glm/geometric.hpp>

BoundingBox::BoundingBox()
	: m_min(0, 0, 0)
	, m_max(0, 0, 0)
{}

BoundingBox::BoundingBox(const glm::vec3& a_min, const glm::vec3& a_max) 
	: m_min{ a_min }
	, m_max{ a_max }
{}

bool BoundingBox::intersectsBoundingBox(const BoundingBox& a_other)const
{
	return	(m_min.x <= a_other.m_max.x && m_max.x >= a_other.m_min.x) &&
			(m_min.y <= a_other.m_max.y && m_max.y >= a_other.m_min.y) &&
			(m_min.z <= a_other.m_max.z && m_max.z >= a_other.m_min.z);
}

bool BoundingBox::intersectsSphere(const glm::vec3& a_pos, float a_radius)const
{
	float dmin = 0;
	for (size_t i = 0; i < 3; i++)
	{
		if (a_pos[i] < m_min[i]) {
			float toAdd = a_pos[i] - m_min[i];
			dmin += toAdd * toAdd;
		}
		else
		{
			if (a_pos[i] > m_max[i]) {
				float toAdd = a_pos[i] - m_max[i];
				dmin += toAdd * toAdd;
			}
		}
	}
	return (dmin <= a_radius * a_radius);
}

bool BoundingBox::fitsEntirelyWithin(const BoundingBox& a_box)const
{
	return (((m_min.x >= a_box.m_min.x && m_min.x <= a_box.m_max.x) && (m_max.x >= a_box.m_min.x && m_max.x <= a_box.m_max.x)) &&
			((m_min.y >= a_box.m_min.y && m_min.y <= a_box.m_max.y) && (m_max.y >= a_box.m_min.y && m_max.y <= a_box.m_max.y)) &&
			((m_min.z >= a_box.m_min.z && m_min.z <= a_box.m_max.z) && (m_max.z >= a_box.m_min.z && m_max.z <= a_box.m_max.z)));
}

glm::vec3 BoundingBox::getCenter()const
{
	return m_min + (m_max - m_min) * 0.5f;
}