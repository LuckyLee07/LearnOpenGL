#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{   
    TexCoords = aTexCoord;
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}

#shader fragment
#version 330 core

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform Light light;
uniform vec3 cameraPos;

uniform samplerCube skybox;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specula1;
uniform sampler2D texture_reflect1;

void main()
{   
    // 环境光影计算
    vec4 diffuseColor = texture(texture_diffuse1, TexCoords);
    
    float relefctIntensity = texture(texture_reflect1, TexCoords).r; 
    vec4 reflectColor = vec4(0.0, 0.0, 0.0, 0.0);
    if(relefctIntensity > 0.1) // 决定是否开启
    {
        vec3 I_viewDir = normalize(FragPos - cameraPos);
        vec3 R_reflectDir = reflect(I_viewDir, normalize(Normal));
        reflectColor = texture(skybox, R_reflectDir) * relefctIntensity; // 使用反射向量采样环境纹理 使用强度系数控制
    }
    
    FragColor = diffuseColor + reflectColor;
}