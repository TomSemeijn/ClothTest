#pragma once
#include "DebugDrawable.h"
#include <glm/vec3.hpp>

class Point : public DebugDrawable
{
public:
    static glm::vec3 s_gravity;
    static glm::vec3 s_globalForces;

    Point(const glm::vec3& a_initialPos, const glm::vec3& a_initialForce, float a_mass);

    Point() = default;
    Point(const Point&) = delete;
    Point& operator=(Point&&) = default;
    Point& operator=(const Point&) = delete;

    const glm::vec3& getPos()const;
    const glm::vec3& getPreviousPos()const;
    const glm::vec3& getForce()const;
    float getInvMass()const;

    void addForce(const glm::vec3& a_toAdd);
    void setForce(const glm::vec3& a_newForce);

    void setPos(const glm::vec3& a_newPos);
    void resetPrevious();
    void pin();
    void unpin();

    void move(float a_deltaTime);

    void draw(const Camera& a_camera)const;

private:
    glm::vec3 m_pos;
    glm::vec3 m_previousPos;
    glm::vec3 m_force;
    float m_mass;
    float m_invMass;
};
