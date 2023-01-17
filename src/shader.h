#pragma once

#include <string>

struct ShaderProgramSource
{
    std::string vertexSource;
    std::string fragmentSource;
};

class Shader
{
public:
    Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
    ~Shader();
    void bind() const;
    void unbind() const;
    // void setBool(const std::string& name, bool value) const;
    // void setFloat(const std::string& name, float value) const;
    // void setInt(const std::string& name, int value) const;
    void setUniform4f(const std::string& name, float x, float y, float z, float w) const;
    void setUniform1f(const std::string& name, float value);
private:
    unsigned int m_programId;
    ShaderProgramSource parseShader(
        const std::string& vertexShaderPath,
        const std::string& fragmentShaderPath
    );
    int createProgramWithShader(
        const std::string& vertexShader,
        const std::string& fragmentShader
    );
    unsigned int createShader(unsigned int type, const std::string& source);
};
