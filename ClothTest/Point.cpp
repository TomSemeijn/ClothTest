#include "Point.h"
#include <glad/glad.h>

glm::vec3 Point::s_gravity(0.f, -9.8f, 0.f);
glm::vec3 Point::s_globalForces(0.f, 0.f, 0.f);

Point::Point(const glm::vec3& a_initialPos, const glm::vec3& a_initialForce, float a_mass)
    : m_pos(a_initialPos)
    , m_previousPos(a_initialPos)
    , m_force(a_initialForce)
    , m_mass(a_mass)
    , m_invMass(a_mass == 0.f ? 0.f : 1.f / a_mass)
{}

const glm::vec3& Point::getPos()const
{
    return m_pos;
}

const glm::vec3& Point::getPreviousPos()const
{
    return m_previousPos;
}

const glm::vec3& Point::getForce()const
{
    return m_force;
}

float Point::getInvMass()const
{
    return m_invMass;
}

void Point::addForce(const glm::vec3& a_toAdd)
{
    m_force += a_toAdd;
}

void Point::setForce(const glm::vec3& a_newForce)
{
    m_force = a_newForce;
}

void Point::setPos(const glm::vec3& a_newPos)
{
    m_pos = a_newPos;
}

void Point::resetPrevious()
{
    m_previousPos = m_pos;
}

void Point::pin()
{
    m_invMass = 0;
}

void Point::unpin()
{
    m_invMass = 1.f / m_mass;
}

void Point::move(float a_deltaTime)
{
    if (m_invMass > 0.00001f)
    {
        glm::vec3 newPos = (m_pos * 1.99f) - (m_previousPos * 0.99f) + (m_force + s_globalForces) * a_deltaTime * a_deltaTime * m_invMass + s_gravity * a_deltaTime * a_deltaTime;
        m_previousPos = m_pos;
        m_pos = newPos;
    }
}

void Point::draw(const Camera& a_camera)const
{

    float positional_data[216] = {
    -1.0f,-1.0f,-1.0f, 0.f, 1.f, 0.f,
    -1.0f,-1.0f, 1.0f, 0.f, 1.f, 0.f,
    -1.0f, 1.0f, 1.0f, 0.f, 1.f, 0.f,
    1.0f, 1.0f,-1.0f,  0.f, 1.f, 0.f,
    -1.0f,-1.0f,-1.0f, 0.f, 1.f, 0.f,
    -1.0f, 1.0f,-1.0f, 0.f, 1.f, 0.f,
    1.0f,-1.0f, 1.0f,  0.f, 1.f, 0.f,
    -1.0f,-1.0f,-1.0f, 0.f, 1.f, 0.f,
    1.0f,-1.0f,-1.0f,  0.f, 1.f, 0.f,
    1.0f, 1.0f,-1.0f,  0.f, 1.f, 0.f,
    1.0f,-1.0f,-1.0f,  0.f, 1.f, 0.f,
    -1.0f,-1.0f,-1.0f, 0.f, 1.f, 0.f,
    -1.0f,-1.0f,-1.0f, 0.f, 1.f, 0.f,
    -1.0f, 1.0f, 1.0f, 0.f, 1.f, 0.f,
    -1.0f, 1.0f,-1.0f, 0.f, 1.f, 0.f,
    1.0f,-1.0f, 1.0f,  0.f, 1.f, 0.f,
    -1.0f,-1.0f, 1.0f, 0.f, 1.f, 0.f,
    -1.0f,-1.0f,-1.0f, 0.f, 1.f, 0.f,
    -1.0f, 1.0f, 1.0f, 0.f, 1.f, 0.f,
    -1.0f,-1.0f, 1.0f, 0.f, 1.f, 0.f,
    1.0f,-1.0f, 1.0f,  0.f, 1.f, 0.f,
    1.0f, 1.0f, 1.0f,  0.f, 1.f, 0.f,
    1.0f,-1.0f,-1.0f,  0.f, 1.f, 0.f,
    1.0f, 1.0f,-1.0f,  0.f, 1.f, 0.f,
    1.0f,-1.0f,-1.0f,  0.f, 1.f, 0.f,
    1.0f, 1.0f, 1.0f,  0.f, 1.f, 0.f,
    1.0f,-1.0f, 1.0f,  0.f, 1.f, 0.f,
    1.0f, 1.0f, 1.0f,  0.f, 1.f, 0.f,
    1.0f, 1.0f,-1.0f,  0.f, 1.f, 0.f,
    -1.0f, 1.0f,-1.0f, 0.f, 1.f, 0.f,
    1.0f, 1.0f, 1.0f,  0.f, 1.f, 0.f,
    -1.0f, 1.0f,-1.0f, 0.f, 1.f, 0.f,
    -1.0f, 1.0f, 1.0f, 0.f, 1.f, 0.f,
    1.0f, 1.0f, 1.0f,  0.f, 1.f, 0.f,
    -1.0f, 1.0f, 1.0f, 0.f, 1.f, 0.f,
    1.0f,-1.0f, 1.0f,  0.f, 1.f, 0.f
    };

    for (size_t k = 0; k < 36; k++)
    {
        size_t base = k * 6;
        for (size_t axis = 0; axis < 3; axis++)
        {
            positional_data[base + axis] = m_pos[axis] + positional_data[base + axis] * 0.125f * 0.75f;
        }
    }

    drawData(a_camera, GL_TRIANGLES, sizeof(positional_data), positional_data, 36);
}
