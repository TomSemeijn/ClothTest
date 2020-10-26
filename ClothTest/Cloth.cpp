#include "Cloth.h"
#include <vector>
#include <array>
#include <glm/geometric.hpp>
#include <glm/gtx/normal.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glad/glad.h>
#include "Camera.h"

#include "Point.h"
#include "Constraint.h"
#include "Sphere.h"
#include "BVH.h"
#include "Basis.h"

#include "Shader.h"
#include "VertexLayout.h"
#include "Texture.h"

#include "Input.h"
#include "KeyboardKey.h"

Shader* Cloth::s_shader = nullptr;
VertexLayout* Cloth::s_vertexLayout = nullptr;
Texture* Cloth::s_cellShadingTexture = nullptr;

Cloth::Cloth(const glm::vec<2, size_t>& a_gridSize, const glm::vec3& a_centerPos, const Basis& a_basis, const float a_particleDistance, const float a_particleMass, const float a_maxParticleStretch)
    : GRID_SIZE(a_gridSize)
    , m_points(nullptr)
    , m_constraints(nullptr)
    , m_bvh(nullptr)
    , m_timer(0)
    , m_restingDistance(a_particleDistance)
    , m_maxDistance(a_particleDistance + a_maxParticleStretch)
    , m_sqrRestingDistance(a_particleDistance * a_particleDistance)
    , m_buffer(0)
    , m_indexBuffer(0)
    , m_initialRenderTarget({ 64, 64 })
{
    const size_t DIM_X = GRID_SIZE.x;
    const size_t DIM_Y = GRID_SIZE.y;
    const size_t DIM_W = DIM_X - 1;
    const size_t DIM_H = DIM_Y - 1;
    m_pointCount = DIM_X * DIM_Y;
    m_constraintCount = (2 * DIM_W * DIM_H) + DIM_W + DIM_H;

    glm::vec3 basePos = a_centerPos - ( 0.5f * glm::vec3(static_cast<float>(GRID_SIZE.x), static_cast<float>(GRID_SIZE.y), 0.f));
    basePos = (basePos.x * a_basis.m_right) + (basePos.y * a_basis.m_up) + (basePos.z * a_basis.m_forward);

    //add all points and constraints
    size_t currentPoint = 0;
    size_t currentConstraint = 0;

    m_points = new Point[m_pointCount];
    m_constraints = new Constraint[m_constraintCount];

    for (size_t x = 0; x < DIM_X; x++)
    {
        for (size_t y = 0; y < DIM_Y; y++)
        {
            m_points[currentPoint] = Point(
                basePos + (a_basis.m_right * ((float)x * a_particleDistance)) + (a_basis.m_up * ((float)y * a_particleDistance)), //pos
                glm::vec3(0.f, 0.f, 0.f), //force
                a_particleMass //mass
            );

            //structural constraint to the left
            if (x > 0)
            {
                size_t leftNeighbour = currentPoint - DIM_Y;
                auto& p1 = m_points[leftNeighbour];
                auto& p2 = m_points[currentPoint];
                float length = glm::length(p1.getPos() - p2.getPos());
                m_constraints[currentConstraint] = Constraint(p1, p2, length, length + a_maxParticleStretch, 1.f);
                currentConstraint++;
            }

            //structural constraint to up
            if (y > 0)
            {
                size_t upNeighbour = currentPoint - 1;
                auto& p1 = m_points[upNeighbour];
                auto& p2 = m_points[currentPoint];
                float length = glm::length(p1.getPos() - p2.getPos());
                m_constraints[currentConstraint] = Constraint(p1, p2, length, length + a_maxParticleStretch, 1.f);
                currentConstraint++;
            }

            currentPoint++;

        }
    }

    printf("Point count: %zu; Constraint count: %zu\n", m_pointCount, m_constraintCount);

    std::vector<Point*> pointVec;
    pointVec.reserve(m_pointCount);
    for (size_t k = 0; k < m_pointCount; k++)
    {
        pointVec.push_back(&m_points[k]);
    }
    m_bvh = new BVH(pointVec);

    if (!s_shader)
    {
        createRenderingResources();
    }
    glGenBuffers(1, &m_buffer);
    glGenBuffers(1, &m_indexBuffer);

    m_vertices = new VertexNode[m_pointCount];
    std::vector<unsigned int> indices;

    size_t current = 0;
    for (size_t x = 0; x < GRID_SIZE.x; x++)
    {
        for (size_t y = 0; y < GRID_SIZE.y; y++)
        {

            if (x > 0 && y > 0)
            {
                size_t leftNeighbour = current - GRID_SIZE.y;
                size_t upNeighbour = current - 1;

                std::array<size_t, 3> thisTriangle{ leftNeighbour, upNeighbour, current };

                m_triangles.emplace_back();
                auto& triangle = m_triangles.back();
                for (size_t newVert = 0; newVert < 3; newVert++)
                {
                    size_t addedVertIndex = thisTriangle[newVert];
                    indices.push_back(addedVertIndex);
                    triangle.vertices[newVert] = addedVertIndex;
                    m_vertices[addedVertIndex].triangles.push_back(m_triangles.size() - 1);
                }
            }

            if (x < GRID_SIZE.x - 1 && y < GRID_SIZE.y - 1)
            {
                size_t rightNeighbour = current + GRID_SIZE.y;
                size_t downNeighbour = current + 1;

                std::array<size_t, 3> thisTriangle{ rightNeighbour, downNeighbour, current };

                m_triangles.emplace_back();
                auto& triangle = m_triangles.back();
                for (size_t newVert = 0; newVert < 3; newVert++)
                {
                    size_t addedVertIndex = thisTriangle[newVert];
                    indices.push_back(addedVertIndex);
                    triangle.vertices[newVert] = addedVertIndex;
                    m_vertices[addedVertIndex].triangles.push_back(m_triangles.size() - 1);
                }

            }

            current++;
        }
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);
}

Cloth::~Cloth()
{
    if (m_buffer)
    {
        glDeleteBuffers(1, &m_buffer);
    }
    delete[] m_vertices;
    m_vertices = nullptr;
    delete m_bvh;
    m_bvh = nullptr;
}

Point& Cloth::getPointAt(size_t a_x, size_t a_y)
{
    return const_cast<Point&>(static_cast<const Cloth*>(this)->getPointAt(a_x, a_y));
}

const Point& Cloth::getPointAt(size_t a_x, size_t a_y)const
{
    return m_points[(a_x * GRID_SIZE.y) + a_y];
}

void Cloth::update(float a_deltaTime)
{
    m_timer += a_deltaTime;
    while (m_timer >= FIXED_TIMESTEP)
    {
        m_timer -= FIXED_TIMESTEP;
        for (size_t k = 0; k < m_pointCount; k++)
        {
            m_points[k].move(FIXED_TIMESTEP);
        }
        for (size_t k = 0; k < NUM_ITERATIONS; k++)
        {
            m_bvh->update();

            for (size_t k = 0; k < m_constraintCount; k++)
            {
                m_constraints[k].satisfy();
            }

            struct PointRefs
            {
                Point* m_p1;
                Point* m_p2;
            };
            std::vector<PointRefs> tempConstraints;
            size_t totalChecked = 0;
            for (size_t i = 0; i < m_pointCount; i++)
            {
                auto& p1 = m_points[i];
                const glm::vec3 pointDstVec(m_sqrRestingDistance, m_sqrRestingDistance, m_sqrRestingDistance);
                BoundingBox testBox{ p1.getPos() - pointDstVec, p1.getPos() + pointDstVec };
                auto foundPoints = m_bvh->getPayloadsWithinBox(testBox);
                for (auto& p2Ptr : foundPoints)
                {
                    totalChecked++;
                    if (&p1 == p2Ptr) { continue; }
                    auto& p2 = *p2Ptr;
                    auto diff = p1.getPos() - p2.getPos();
                    if (glm::dot(diff, diff) <= m_sqrRestingDistance)
                    {
                        tempConstraints.push_back(PointRefs{ &p1, &p2 });
                    }
                }
            }

            //printf("Checked an average of %F times\n", static_cast<float>(totalChecked) / m_pointCount);
            for (auto& points : tempConstraints)
            {
                Constraint tempConstraint(*points.m_p1, *points.m_p2, m_maxDistance, m_maxDistance, 1.f);
                tempConstraint.satisfy();
            }

            m_bvh->update();

            for (auto& sphere : m_spheres)
            {
                auto points = m_bvh->getPayloadsWithinSphere(*sphere);
                if (!points.empty())
                {
                    const float sqrRadius = sphere->getRadius() * sphere->getRadius();
                    for (auto& point : points)
                    {
                        const auto diff = point->getPos() - sphere->getPos();
                        const float sqrDst = glm::dot(diff, diff);
                        if (sqrDst < sqrRadius)
                        {
                            const float dst = sqrtf(sqrDst);
                            point->setPos(point->getPos() + (diff / dst) * (sphere->getRadius() - dst));
                        }
                    }
                    m_bvh->update(); //update BVH again because points were moved
                }
            }
        }
    }
}

void Cloth::addSphere(Sphere& a_sphere)
{
    m_spheres.push_back(&a_sphere);
}

void Cloth::removeSphere(Sphere& a_sphere)
{
    m_spheres.erase(std::find(m_spheres.begin(), m_spheres.end(), &a_sphere));
}

void Cloth::drawSimulation(const Camera& a_camera, bool a_persistent)const
{

    if (a_persistent)
    {
        glDisable(GL_DEPTH_TEST);
    }

    for (size_t k = 0; k < m_pointCount; k++)
    {
        m_points[k].draw(a_camera);
    }

    for (size_t k = 0; k < m_constraintCount; k++)
    {
        m_constraints[k].draw(a_camera);
    }

    if (a_persistent)
    {
        glEnable(GL_DEPTH_TEST);
    }

}

void Cloth::drawBVH(const Camera& a_camera, bool a_persistent)const
{
    m_bvh->draw(a_camera, a_persistent);
}

void Cloth::draw(const Camera& a_camera)const
{
    std::vector<float> data;

    for (size_t k = 0; k < m_pointCount; k++)
    {
        VertexNode& node = m_vertices[k];
        glm::vec3 normal(0, 0, 0);
        for (auto& triangleindex : node.triangles)
        {
            auto& triangle = m_triangles[triangleindex];
            //normal calculation from https://www.khronos.org/opengl/wiki/Calculating_a_Surface_Normal
            glm::vec3 u = m_points[triangle.vertices[1]].getPos() - m_points[triangle.vertices[0]].getPos();
            glm::vec3 v = m_points[triangle.vertices[2]].getPos() - m_points[triangle.vertices[0]].getPos();
            //printf("u = { %F, %F, %F } v = { %F, %F, %F }\n", u.x, u.y, u.z, v.x, v.y, v.z);
            normal += glm::vec3((u.y * v.z) - (u.z * v.y), (u.z * v.x) - (u.x * v.z), (u.x * v.y) - (u.y * v.x));
        }
        normal = glm::normalize(normal / static_cast<float>(node.triangles.size()));

        for (size_t axis = 0; axis < 3; axis++)
        {
            data.push_back(m_points[k].getPos()[axis]);
        }

        for (size_t axis = 0; axis < 3; axis++)
        {
            data.push_back(normal[axis]);
        }

    }

    glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.size(), data.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);

    s_shader->bind();
    s_shader->setUniform("u_vp", a_camera.getView() * a_camera.getProjection());
    s_vertexLayout->bind();
    glDrawElements(GL_TRIANGLES, getIndexCount(), GL_UNSIGNED_INT, (void*)0);

}

void Cloth::createRenderingResources()
{
    s_shader = new Shader(
        "#version 330 core\n"
        ""
        "in vec3 a_pos;"
        "in vec3 a_normal;"
        ""
        "uniform mat4 u_vp;"
        ""
        "out vec3 v_worldPos;"
        "out vec3 v_normal;"
        ""
        "void main()"
        "{"
        "   gl_Position = u_vp * vec4(a_pos, 1.0);"
        "   v_worldPos = a_pos;"
        "   v_normal = a_normal;"
        "}"

        ,

        "#version 330 core\n"
        ""
        "in vec3 v_worldPos;"
        "in vec3 v_normal;"
        ""
        "uniform mat4 u_vp;"
        "uniform vec3 u_color;"
        "uniform sampler2D u_cellShadingTexture;"
        ""
        "out vec4 outColor;"
        ""
        ""
        "vec3 rgb2hsv(vec3 c)"
        "{"
        "    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);"
        "    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));"
        "    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));"
        ""
        "    float d = q.x - min(q.w, q.y);"
        "    float e = 1.0e-10;"
        "    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);"
        "}"
        ""
        "vec3 hsv2rgb(vec3 c)"
        "{"
        "    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);"
        "    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);"
        "    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);"
        "}"
        ""
        "int roundUp(int numToRound, int multiple)"
        "{"
        "    return ((numToRound + multiple - 1) / multiple) * multiple;"
        "}"
        ""
        "void main()"
        "{"
        "   vec3 lightPos = vec3(10.0, 20.0, -5.0);"
        "   vec3 lightDir = normalize(v_worldPos - lightPos);"
        "   vec3 normal = v_normal;"
        "   if(dot(vec3(u_vp * vec4(normal, 0.0)), vec3(0.0, 0.0, 1.0)) < 0.0){ normal *= -1; }"
        "   float lightIntensity = clamp(dot(normal, lightDir), 0.0, 1.0);"
        ""
        "   vec3 finalColor = u_color * lightIntensity;"
        "   vec3 hsvColor = rgb2hsv(finalColor);"
        "   finalColor = hsv2rgb(vec3(hsvColor.rg, texture(u_cellShadingTexture, vec2(hsvColor.b, 0.0))));"
        ""
        "   outColor = vec4(finalColor, 1.0);"
        "}"
    );

    s_shader->setUniform("u_color", glm::vec3(1.f, 1.f, 1.f));

    s_vertexLayout = new VertexLayout();
    s_vertexLayout->addFloatComponent(3); //pos
    s_vertexLayout->addFloatComponent(3); //normal

    s_cellShadingTexture = new Texture("Assets/CellShading.png");
    s_shader->setUniform("u_cellShadingTexture", *s_cellShadingTexture, 1);
}