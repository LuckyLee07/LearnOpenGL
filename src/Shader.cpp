#include "Shader.h"
#include "Common.h"
#include "glm/gtc/type_ptr.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const std::string& filepath)
{
    ShaderSource source = ParseShader(filepath.c_str());
    m_RenderId = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RenderId));
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RenderId));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
    GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string& name, float value)
{
    GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3)
{
    GLCall(glUniform4f(GetUniformLocation(name), f0, f1, f2, f3));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
    //GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix)));
}

int Shader::GetUniformLocation(const std::string& name)
{
    auto findIter = m_UniformLocationCache.find(name);
    if (findIter != m_UniformLocationCache.end())
    {
        return findIter->second;
    }
    
    int location = glGetUniformLocation(m_RenderId, name.c_str());
    if (location == -1)
        std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
    
    return location;
}

ShaderSource Shader::ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else
        {
            ss[(int)type] << line << "\n";
        }
    }

    return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int shaderId = glCreateShader(type);
    const char* src = source.c_str();
    GLCall(glShaderSource(shaderId, 1, &src, nullptr));
    GLCall(glCompileShader(shaderId));

    GLint result;
    GLCall(glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result));
    if (result != GL_TRUE)
    {
        GLint length;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &result);
        //char* message = (char*)alloca(length * sizeof(char));
        //second args: maxLength
        //glGetShaderInfoLog(shaderId, length, &length, message);
        char message[1024];
        glGetShaderInfoLog(shaderId, 1024, &length, message);

        const char* sType = type == GL_VERTEX_SHADER ? "vertex" : "fragment";
        std::cout << "Failed to compile " << sType << "shader!" << std::endl;
        std::cout << message << std::endl;

        glDeleteShader(shaderId);
        return 0;
    }
    return shaderId;
}


unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int programId = GLCall(glCreateProgram());
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCall(glAttachShader(programId, vs));
    GLCall(glAttachShader(programId, fs));
    GLCall(glLinkProgram(programId));
    GLCall(glValidateProgram(programId));

    //glDetachShader/glDeleteShader：Detach后无源码调试麻烦
    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return programId;
}
