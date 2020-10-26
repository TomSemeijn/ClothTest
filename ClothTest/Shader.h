#pragma once
#include <string>
#include <map>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

typedef unsigned int GLuint;

class Shader
{
public:
    /// @brief Constructor
    /// @param a_vertexSource Source code string of the vertex shader
    /// @param a_fragmentSource Source code string of the fragment shader
    Shader(const std::string& a_vertexSource, const std::string& a_fragmentSource);

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    /// @brief Move constructor
    /// @param a_toMove The shader to move
    Shader(Shader&& a_toMove);

    /// @brief Move assignment
    /// @param a_toMove The shader to move
    /// @return Reference to self for chaining purposes
    Shader& operator=(Shader&& a_toMove);

    /// @brief Destructor
    ~Shader();

    /// @brief Binds the shader to be the currently used shader
    void bind();

    /// @brief Sets a float uniform
    /// @param a_name The name of the uniform
    /// @param a_value The new value of the uniform
    void setUniform(const std::string& a_name, const float a_value);

    /// @brief Sets an int uniform
    /// @param a_name The name of the uniform
    /// @param a_value The new value of the uniform
    void setUniform(const std::string& a_name, const int a_value);

    /// @brief Sets a bool uniform
    /// @param a_name The name of the uniform
    /// @param a_value The new value of the uniform
    void setUniform(const std::string& a_name, const bool a_value);

    /// @brief Sets a vec2 uniform
    /// @param a_name The name of the uniform
    /// @param a_value The new value of the uniform
    void setUniform(const std::string& a_name, const glm::vec2& a_value);

    /// @brief Sets a vec3 uniform
    /// @param a_name The name of the uniform
    /// @param a_value The new value of the uniform
    void setUniform(const std::string& a_name, const glm::vec3& a_value);

    /// @brief Sets a vec4 uniform
    /// @param a_name The name of the uniform
    /// @param a_value The new value of the uniform
    void setUniform(const std::string& a_name, const glm::vec4& a_value);

    /// @brief Sets a mat3 uniform
    /// @param a_name The name of the uniform
    /// @param a_value The new value of the uniform
    void setUniform(const std::string& a_name, const glm::mat3& a_value);

    /// @brief Sets a mat4 uniform
    /// @param a_name The name of the uniform
    /// @param a_value The new value of the uniform
    void setUniform(const std::string& a_name, const glm::mat4& a_value);

    /// @brief Sets a texture uniform
    /// @param a_name The name of the uniform
    /// @param a_value The new value of the uniform
    void setUniform(const std::string& a_name, class Texture& a_value, unsigned int a_slot = 0);

private:
    //handle to the shader program
    GLuint m_shaderProgram;

    //a map for storing uniform locations
    std::map<std::string, GLuint> m_uniformLocations;

    //returns the location of the uniform of the given name
    GLuint getUniformLocation(const std::string& a_name);

    void createShaderProgram(const std::string& a_vertexSource, const std::string& a_fragmentSource);

};
