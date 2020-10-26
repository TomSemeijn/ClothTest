#pragma once

typedef unsigned int GLuint;
typedef unsigned int GLenum;

class Shader;
class VertexLayout;
class Camera;

class DebugDrawable
{
public:
    DebugDrawable& operator=(DebugDrawable&& a_toMove);
    ~DebugDrawable();

protected:
    DebugDrawable();

    void drawData(const Camera& a_camera, GLenum a_type, size_t a_size, void* a_data, size_t a_vertexCount)const;

private:
    static Shader* s_shader;
    static VertexLayout* s_layout;
    static void createRenderingResources();

    GLuint m_buffer;
};