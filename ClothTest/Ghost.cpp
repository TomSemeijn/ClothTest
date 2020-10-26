#include "Ghost.h"
#include <glad/glad.h>
#include "Input.h"
#include "KeyboardKey.h"
#include "Shader.h"
#include "Point.h"
#include "Basis.h"

Ghost::Ghost(Input& a_input, const glm::vec3& a_initialPos)
	: m_input(a_input)
	, m_transf(a_initialPos, glm::angleAxis(glm::radians(-90.f), glm::vec3(1.f, 0.f, 0.f)), glm::vec3(1, 1, 1))
	, m_baseZ(a_initialPos.z)
	, m_head(3.f)
	, m_leftHand(2.f)
	, m_rightHand(2.f)
	, m_body(4.f)
	, m_tail(2.f)
	, m_timer(0.f)
	, m_tailPointDirections{
		glm::normalize(glm::vec2(-1.f,  1.f)),
		glm::normalize(glm::vec2( 1.f,  1.f)),
		glm::normalize(glm::vec2(-1.f, -1.f)),
		glm::normalize(glm::vec2( 1.f, -1.f)),
		glm::normalize(glm::vec2( 0.f,  -1.f))
	  }
	, m_cloth(
		{ 44, 26 }, //grid size
		m_transf.toWorldPos(glm::vec3(0.f, 0.f, m_head.getRadius() + 0.5f)), //base pos
		Basis{		//basis
			glm::vec3(1.f, 0.f, 0.f),
			glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(0.f, 1.f, 0.f)
		},	
		1.f,		//particle distance
		1.f,		//particle mass
		0.005f		//contraint max stretch
	  )
{
	m_head.setPos(a_initialPos);
	auto& centerPoint = m_cloth.getPointAt(m_cloth.GRID_SIZE.x / 2, m_cloth.GRID_SIZE.y / 2);
	centerPoint.pin();
	m_headPoint = &centerPoint;
	m_leftHandPoint = &m_cloth.getPointAt(m_cloth.GRID_SIZE.x / 2 - 8, m_cloth.GRID_SIZE.y / 2 - 4);
	m_rightHandPoint = &m_cloth.getPointAt(m_cloth.GRID_SIZE.x / 2 + 8, m_cloth.GRID_SIZE.y / 2 - 4);
	for (size_t k = 0; k < TAIL_POINT_COUNT; k++)
	{
		auto& dir = m_tailPointDirections[k];
		auto& target = m_tailPoints[k];
		const size_t offset = dir.x == 0 || dir.y == 0 ? 0 : 4;
		size_t indexX = dir.x < 0 ? offset : (fabsf(dir.x) < 0.1f ? m_cloth.GRID_SIZE.x / 2 : (m_cloth.GRID_SIZE.x - 1) - offset);
		size_t indexY = dir.y < 0 ? offset : (fabsf(dir.y) < 0.1f ? m_cloth.GRID_SIZE.y / 2 : (m_cloth.GRID_SIZE.y - 1) - offset);
		target = &m_cloth.getPointAt(indexX, indexY);
	}

	m_cloth.addSphere(m_head);
	m_cloth.addSphere(m_leftHand);
	m_cloth.addSphere(m_rightHand);
	m_cloth.addSphere(m_body);
	m_cloth.addSphere(m_tail);

	m_head.setPos(m_transf.getPos());
	m_headPoint->setPos(m_transf.toWorldPos(glm::vec3(0.f, 0.f, m_head.getRadius())));
	m_leftHandPoint->setPos(m_leftHand.getPos() + (m_transf.getForward() * m_leftHand.getRadius()));
	m_rightHandPoint->setPos(m_rightHand.getPos() + (m_transf.getForward() * m_rightHand.getRadius()));

	Point::s_gravity = glm::vec3(0, 0, 0);
	float timer = 0.f;
	while (timer < 0.5f)
	{
		timer += m_cloth.FIXED_TIMESTEP;
		update(m_cloth.FIXED_TIMESTEP);
	}
	for (size_t k = 0; k < TAIL_POINT_COUNT; k++)
	{
		m_tailPoints[k]->pin();
	}
}

Ghost::~Ghost()
{
	m_cloth.removeSphere(m_head);
}

void Ghost::draw(const Camera& a_camera)const
{
	m_cloth.draw(a_camera);
	//m_head.draw(a_camera);
	//m_leftHand.draw(a_camera);
	//m_rightHand.draw(a_camera);
	//m_body.draw(a_camera);
	//m_tail.draw(a_camera);

	float pointToDraw[6]{
		m_headPoint->getPos().x,
		m_headPoint->getPos().y,
		m_headPoint->getPos().z,
		1.f, 0.f, 0.f
	};
	glDisable(GL_DEPTH_TEST);
	drawData(a_camera, GL_POINTS, sizeof(pointToDraw), pointToDraw, 1);
	glEnable(GL_DEPTH_TEST);
}

void Ghost::update(float a_deltaTime, bool a_updateTail)
{
	updateMovement(a_deltaTime);
	m_head.setPos(m_transf.getPos());
	m_headPoint->setPos(m_transf.toWorldPos(glm::vec3(0.f, 0.f, m_head.getRadius())));

	const float handDstX = (m_head.getRadius() + m_leftHand.getRadius() + 1.f);
	const float handDstY = (m_head.getRadius() + m_leftHand.getRadius() + 2.f);
	m_leftHand.setPos(m_transf.getPos() + (-m_transf.getForward() * handDstY) + (-m_transf.getRight() * handDstX));
	m_rightHand.setPos(m_transf.getPos() + (-m_transf.getForward() * handDstY) + (m_transf.getRight() * handDstX));
	m_leftHandPoint->setPos(m_leftHand.getPos() + (m_transf.getForward() * m_leftHand.getRadius()));
	m_rightHandPoint->setPos(m_rightHand.getPos() + (m_transf.getForward() * m_rightHand.getRadius()));

	const float bodyDst = (m_head.getRadius() + m_body.getRadius());
	m_body.setPos(m_transf.getPos() + (-m_transf.getForward() * bodyDst));

	const float tailDst = (bodyDst + m_body.getRadius() + m_tail.getRadius());
	m_tail.setPos(m_transf.getPos() + (-m_transf.getForward() * tailDst));

	for (size_t k = 0; k < TAIL_POINT_COUNT; k++)
	{
		auto& dir = m_tailPointDirections[k];
		m_tailPoints[k]->setPos(m_tail.getPos() + (-m_transf.getForward() * m_tail.getRadius()) + (m_transf.getRight() * dir.x * m_tail.getRadius()) + (-m_transf.getUp() * dir.y * m_tail.getRadius()));
	}

	m_timer += a_deltaTime;
	Point::s_globalForces = glm::vec3(sinf(m_timer * 2.f) * 5.f, 0.f, 0.f);
	m_cloth.update(a_deltaTime);
}

void Ghost::updateMovement(float a_deltaTime)
{
	glm::vec2 toMove(0, 0);
	if (m_input.isKeyPressed(KeyboardKey::KEY_A))
	{
		toMove.x -= 1;
	}
	if (m_input.isKeyPressed(KeyboardKey::KEY_D))
	{
		toMove.x += 1;
	}
	if (m_input.isKeyPressed(KeyboardKey::KEY_W))
	{
		toMove.y += 1;
	}
	if (m_input.isKeyPressed(KeyboardKey::KEY_S))
	{
		toMove.y -= 1;
	}
	if (glm::dot(toMove, toMove) > 0.001f)
	{
		const float rotateSpeedMultiplier = 1.f;
		const float movementSpeedMultiplier = 10.f;
		toMove = glm::normalize(toMove) * a_deltaTime * movementSpeedMultiplier;
		auto& currentPos = m_transf.getPos();
		m_transf.slerpRotationTowardsWorldPos(glm::vec3(glm::vec2(currentPos.x, currentPos.y) + (toMove), m_baseZ), a_deltaTime * rotateSpeedMultiplier);
		m_transf.translate(m_transf.getForward() * a_deltaTime * movementSpeedMultiplier);
	}
}
