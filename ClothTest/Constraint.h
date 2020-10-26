#pragma once
#include "DebugDrawable.h"

class Point;

class Constraint : public DebugDrawable
{
public:
    Constraint(Point& a_point1, Point& a_point2, float a_restLength, float a_maxLength, float a_bendCoefficient);

    Constraint();
    Constraint(const Constraint&) = delete;
    Constraint(Constraint&&) = default;
    Constraint& operator=(Constraint&&) = default;
    Constraint& operator=(const Constraint&) = delete;

    void satisfy();

    void draw(const Camera& a_camera)const;

private:
    Point* m_point1;
    Point* m_point2;

    float m_restLength;
    float m_sqrRestLength;
    float m_maxLength;
    float m_sqrMaxLength;
    float m_bendCoefficient;

};
