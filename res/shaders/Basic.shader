#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

uniform vec3 lightPos;

out vec3 Normal;
out vec3 FragPos;
out vec3 LightPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    //Normal = vec3(view * model * vec4(aNormal, 1.0));
    mat3 normalMatrix = mat3(view * transpose(inverse(model)));
    Normal = normalMatrix * aNormal;
    
    FragPos = vec3(view * model * vec4(aPos, 1.0));
    LightPos = vec3(view * vec4(lightPos, 1.0));
}

#shader fragment
#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec3 LightPos;

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
//uniform vec3 viewPos;

uniform float ambientStrength;
uniform int specularFactor;
uniform float specularStrength;

void main()
{   
    // 环境光影计算
    //float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    
    // 漫反射光源
    vec3 norml = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos);

    float diff = max(dot(norml, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    //镜面反射计算
    //float specularStrength = 0.5;
    vec3 viewDir = normalize(- FragPos);
    vec3 reflectDir = reflect(-lightDir, norml);

    //int specularFactor = 32;
    float sepc = pow(max(dot(viewDir, reflectDir), 0.0), specularFactor);    
    vec3 specular = specularStrength * sepc * lightColor;
    
    vec3 result = (ambient + diffuse + specular) * objectColor;

    FragColor = vec4(result, 1.0);
}