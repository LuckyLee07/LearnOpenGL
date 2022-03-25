#ifndef __SHADER_H__
#define __SHADER_H__

#include <string>

struct ShaderSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader
{
private:
    std::string m_Filepath;
    unsigned int m_RenderId;
public:
    Shader(const std::string& filepath);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    //SetUniform-Value
    void SetUniform1f(const std::string& name, float value);
    void SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3);

private:
    int GetUniformLocation(const std::string& name);

    ShaderSource ParseShader(const std::string& filepath);
    unsigned int CompileShader(unsigned int type, const std::string& source);
    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
};

#endif