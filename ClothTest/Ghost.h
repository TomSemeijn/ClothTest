#pragma once
#include "DebugDrawable.h"
#include "Cloth.h"
#include "Sphere.h"
#include "Transform.h"

class Input;
class Camera;
class Point;

class Ghost : public DebugDrawable
{
public:
	Ghost(Input& a_input, const glm::vec3& a_initialPos);
	~Ghost();

	void draw(const Camera& a_camera)const;
	void update(float a_deltaTime, bool a_updateTail = true);

private:
	Input& m_input;

	Transform m_transf;
	float m_baseZ;
	Sphere m_head;
	Sphere m_leftHand;
	Sphere m_rightHand;
	Sphere m_body;
	Sphere m_tail;

	Point* m_headPoint;
	Point* m_leftHandPoint;
	Point* m_rightHandPoint;
	static constexpr size_t TAIL_POINT_COUNT = 5;
	Point* m_tailPoints[TAIL_POINT_COUNT];
	glm::vec2 m_tailPointDirections[TAIL_POINT_COUNT];

	float m_timer;

	Cloth m_cloth;

	void updateMovement(float a_deltaTime);

};
