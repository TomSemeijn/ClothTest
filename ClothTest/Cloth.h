#pragma once
#include "DebugDrawable.h"
#include <vector>
#include <glm/detail/type_vec2.hpp>
#include <glm/mat4x4.hpp>
#include "FrameBuffer.h"

class Point;
class Constraint;
class Sphere;
class BVH;
struct Basis;
typedef unsigned int GLuint;

class Cloth : public DebugDrawable
{
public:
    static constexpr size_t NUM_ITERATIONS = 4;
    static constexpr float FIXED_TIMESTEP = 1.f / 60.f;

    const glm::vec<2, size_t> GRID_SIZE;

    Cloth(const glm::vec<2, size_t>& a_gridSize, const glm::vec3& a_centerPos, const Basis& a_basis, float a_particleDistance, float a_particleMass, float a_maxParticleStretch);
    Cloth(const Cloth&) = delete;
    Cloth& operator=(const Cloth&) = delete;
    ~Cloth();

    Point& getPointAt(size_t a_x, size_t a_y);
    const Point& getPointAt(size_t a_x, size_t a_y)const;

    void update(float a_deltaTime);

    void addSphere(Sphere& a_sphere);
    void removeSphere(Sphere& a_sphere);
    
    void draw(const Camera& a_camera)const;
    void drawSimulation(const Camera& a_camera, bool a_persistent = false)const;
    void drawBVH(const Camera& a_camera, bool a_persistent = false)const;

private:
    Point* m_points;
    size_t m_pointCount;
    Constraint* m_constraints;
    size_t m_constraintCount;

    BVH* m_bvh;

    float m_timer;
    float m_restingDistance;
    float m_maxDistance;
    float m_sqrRestingDistance;

    static class Shader* s_shader;
    static class VertexLayout* s_vertexLayout;
    static class Texture* s_cellShadingTexture;

    GLuint m_buffer;
    GLuint m_indexBuffer;
    FrameBuffer m_initialRenderTarget;

    struct VertexNode
    {
        std::vector<size_t> triangles;
    };
    struct TriangleNode
    {
        size_t vertices[3] = { 0 };
    };
    VertexNode* m_vertices;
    std::vector<TriangleNode> m_triangles;

    std::vector<Sphere*> m_spheres;

    static void createRenderingResources();

    size_t getIndexCount()const { return m_triangles.size() * 3; };

};
