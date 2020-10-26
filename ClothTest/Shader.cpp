#include "Shader.h"
#include <glad/glad.h>
#include "Texture.h"

Shader::Shader(const std::string& a_vertexSource, const std::string& a_fragmentSource)
    : m_shaderProgram(0)
{
    createShaderProgram(a_vertexSource, a_fragmentSource);
}

Shader::Shader(Shader&& a_toMove)
    : m_shaderProgram(a_toMove.m_shaderProgram)
{
    a_toMove.m_shaderProgram = 0;
}

Shader& Shader::operator=(Shader&& a_toMove)
{
    m_shaderProgram = a_toMove.m_shaderProgram;
    a_toMove.m_shaderProgram = 0;
    return *this;
}

Shader::~Shader()
{
    if (m_shaderProgram)
    {
        glDeleteProgram(m_shaderProgram);
    }
}

void Shader::bind()
{
    glUseProgram(m_shaderProgram);
}

void Shader::setUniform(const std::string& a_name, const float a_value)
{
    bind();
    glUniform1f(getUniformLocation(a_name), a_value);
}

void Shader::setUniform(const std::string& a_name, const int a_value)
{
    bind();
    glUniform1i(getUniformLocation(a_name), a_value);
}

void Shader::setUniform(const std::string& a_name, const bool a_value)
{
    bind();
    glUniform1i(getUniformLocation(a_name), a_value ? 1 : 0);
}

void Shader::setUniform(const std::string& a_name, const glm::vec2& a_value)
{
    bind();
    glUniform2f(getUniformLocation(a_name), a_value.x, a_value.y);
}

void Shader::setUniform(const std::string& a_name, const glm::vec3& a_value)
{
    bind();
    glUniform3f(getUniformLocation(a_name), a_value.x, a_value.y, a_value.z);
}

void Shader::setUniform(const std::string& a_name, const glm::vec4& a_value)
{
    bind();
    glUniform4f(getUniformLocation(a_name), a_value.x, a_value.y, a_value.z, a_value.w);
}

void Shader::setUniform(const std::string& a_name, const glm::mat3& a_value)
{
    bind();
    glUniformMatrix3fv(getUniformLocation(a_name), 1, GL_FALSE, &a_value[0][0]);
}

void Shader::setUniform(const std::string& a_name, const glm::mat4& a_value)
{
    bind();
    glUniformMatrix4fv(getUniformLocation(a_name), 1, GL_FALSE, &a_value[0][0]);
}

void Shader::setUniform(const std::string& a_name, Texture& a_value, unsigned int a_slot)
{
    bind();
    a_value.bind(a_slot);
    glUniform1i(getUniformLocation(a_name), a_slot);
}

//returns the location of the uniform of the given name
GLuint Shader::getUniformLocation(const std::string& a_name)
{
    auto iter = m_uniformLocations.find(a_name);
    if (iter != m_uniformLocations.end())
    {
        return iter->second;
    }
    GLint location = glGetUniformLocation(m_shaderProgram, a_name.c_str());
    if (location != -1)
    {
        m_uniformLocations.insert({ a_name, location });
    }
    else
    {
        printf("Tried to get location of unknown uniform '%s'!\n", a_name.c_str());
    }
    return location;
}

void Shader::createShaderProgram(const std::string& a_vertexSource, const std::string& a_fragmentSource)
{
    //creates shaders
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    //loads source
    const char* vertexSource = a_vertexSource.c_str();
    const char* fragmentSource = a_fragmentSource.c_str();
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);

    //compiles shaders
    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);

    int compileResult;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compileResult);
    if (!compileResult)
    {
        GLchar message[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, message);

        printf("[OpenGL] Error compiling vertex shader! \n%s\n", message);
        assert(false);
    }
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compileResult);
    if (!compileResult)
    {
        GLchar message[512];
        glGetShaderInfoLog(fragmentShader, 512, NULL, message);

        printf("[OpenGL] Error compiling fragment shader! \n%s\n", message);
        assert(false);
    }

    //creates shader program
    m_shaderProgram = glCreateProgram();
    glAttachShader(m_shaderProgram, vertexShader);
    glAttachShader(m_shaderProgram, fragmentShader);

    //links program
    glLinkProgram(m_shaderProgram);
    int linkResult;
    glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &linkResult);
    if (!linkResult)
    {
        char errorStr[512];
        glGetProgramInfoLog(m_shaderProgram, 512, NULL, errorStr);
        assert(false);
    }

    //deletes shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

