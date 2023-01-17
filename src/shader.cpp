#include <glad/glad.h>
#include "shader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

std::string readFile(const std::string& filepath)
{
    std::ifstream file;
    file.exceptions (std::ifstream::failbit | std::ifstream::badbit);

    std::stringstream stream;
    try
    {
        file.open(filepath);
        stream << file.rdbuf();
        file.close();
    }
    catch(const std::exception& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    return stream.str();
}

Shader::Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
    ShaderProgramSource source = parseShader(vertexShaderPath, fragmentShaderPath);
    m_programId = createProgramWithShader(source.vertexSource, source.fragmentSource);
}

Shader::~Shader()
{
    glDeleteProgram(m_programId);
}

void Shader::bind() const
{
    glUseProgram(m_programId);
}

void Shader::unbind() const
{
    glUseProgram(0);
}

void Shader::setUniform4f(const std::string& name, float x, float y, float z, float w) const
{
    unsigned int location = glGetUniformLocation(m_programId, name.c_str());
    glUniform4f(location, x, y, z, w);
}


ShaderProgramSource Shader::parseShader(
    const std::string& vertexShaderPath,
    const std::string& fragmentShaderPath
)
{
    return {
        readFile(vertexShaderPath),
        readFile(fragmentShaderPath)
    };
}

int Shader::createProgramWithShader(
    const std::string& vertexShaderSource,
    const std::string& fragmentShaderSource
)
{
    m_programId = glCreateProgram();
    unsigned int vertexShaderId = createShader(GL_VERTEX_SHADER, vertexShaderSource);
    unsigned int fragmentShaderId = createShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    glAttachShader(m_programId, vertexShaderId);
    glAttachShader(m_programId, fragmentShaderId);
    glLinkProgram(m_programId);
    glValidateProgram(m_programId);

    int success;
    glGetProgramiv(m_programId, GL_LINK_STATUS, &success);
    if(!success)
    {
        int length;
        glGetProgramiv(m_programId, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetProgramInfoLog(m_programId, length, &length, message);
        std::cout << "Failed to link: " << message << std::endl;
    }

    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);

    return m_programId;
}

unsigned int Shader::createShader(unsigned int type, const std::string& source)
{
    unsigned int shaderId = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shaderId, 1, &src, nullptr);
    glCompileShader(shaderId);

    int result;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(shaderId, length, &length, message);
        std::cout << "Failed to compile "
            << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
            << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(shaderId);
        return 0;
    }
    return shaderId;
}

