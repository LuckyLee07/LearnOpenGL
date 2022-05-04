#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    //Normal = vec3(view * model * vec4(aNormal, 1.0));
    mat3 normalMatrix = mat3(transpose(inverse(model)));
    Normal = normalMatrix * aNormal;
    FragPos = vec3(model * vec4(aPos, 1.0));
    TexCoords = aTexCoords;
}

#shader fragment
#version 330 core

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct Material {
    //vec3 diffuse;
    //vec3 specular;
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

uniform Light light;
uniform Material material;

uniform vec3 viewPos;

void main()
{   
    // 环境光影计算
    //float ambientStrength = 0.1;
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    
    // 漫反射光源
    vec3 norml = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);

    float diff = max(dot(norml, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse, TexCoords)));

    //镜面反射计算
    //float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norml);

    //int specularFactor = 32;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);    
    vec3 specular = light.specular * (spec * vec3(texture(material.specular, TexCoords)));
    
    vec3 result = ambient + diffuse + specular;
    //vec3 result = (ambient + diffuse + specular) * objectColor;

    FragColor = vec4(result, 1.0);
}