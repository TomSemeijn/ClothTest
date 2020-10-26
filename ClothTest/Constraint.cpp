#include "Constraint.h"
#include <glad/glad.h>
#include <glm/geometric.hpp>
#include "Point.h"

Constraint::Constraint()
	: m_point1(nullptr)
	, m_point2(nullptr)
	, m_restLength(0.f)
	, m_sqrRestLength(0.f)
	, m_maxLength(0.f)
	, m_sqrMaxLength(0.f)
	, m_bendCoefficient(0.f)
{}

Constraint::Constraint(Point& a_point1, Point& a_point2, float a_restLength, float a_maxLength, float a_bendCoefficient)
    : m_point1(&a_point1)
    , m_point2(&a_point2)
	, m_restLength(a_restLength)
	, m_sqrRestLength(a_restLength * a_restLength)
	, m_maxLength(a_maxLength)
	, m_sqrMaxLength(a_maxLength * a_maxLength)
	, m_bendCoefficient(a_bendCoefficient)
{}

void Constraint::satisfy()
{
	auto& p1 = m_point1->getPos();
	auto& p2 = m_point2->getPos();
	auto delta = p2 - p1;

	float p1_im = m_point1->getInvMass();
	float p2_im = m_point2->getInvMass();

	float dst = glm::length(delta);
	glm::vec3 correction = (delta / dst) * (dst - m_maxLength * m_restLength) * m_bendCoefficient;
	float m1 = p1_im / (p1_im + p2_im);
	float m2 = p2_im / (p1_im + p2_im);
	if (p1_im != 0.f)
	{
		m_point1->setPos(p1 + (correction * m1));
	}
	if (p2_im != 0.f)
	{
		m_point2->setPos(p2 - (correction * m2));
	}

}

void Constraint::draw(const Camera& a_camera)const
{

	float data[12]{
		m_point1->getPos().x, m_point1->getPos().y, m_point1->getPos().z, 1.f, 0.f, 0.f,
		m_point2->getPos().x, m_point2->getPos().y, m_point2->getPos().z, 1.f, 0.f, 0.f
	};

	drawData(a_camera, GL_LINES, sizeof(data), data, 2);
}
