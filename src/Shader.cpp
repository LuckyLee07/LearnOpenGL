#include "Shader.h"
#include "Renderer.h"

#include <string>
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

void Shader::SetUniform1f(const std::string& name, float value)
{
    GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3)
{
    GLCall(glUniform4f(GetUniformLocation(name), f0, f1, f2, f3));
}

int Shader::GetUniformLocation(const std::string& name)
{
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
    glShaderSource(shaderId, 1, &src, nullptr);
    glCompileShader(shaderId);

    GLint result;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
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
    unsigned int programId = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(programId, vs);
    glAttachShader(programId, fs);
    glLinkProgram(programId);
    glValidateProgram(programId);

    //glDetachShader/glDeleteShader：Detach后无源码调试麻烦
    glDeleteShader(vs);
    glDeleteShader(fs);

    return programId;
}