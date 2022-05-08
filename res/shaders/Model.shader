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
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    mat3 normalMatrix = mat3(transpose(inverse(model)));
    Normal = normalMatrix * aNormal;

    FragPos = vec3(model * vec4(aPos, 1.0));
    TexCoords = aTexCoord;
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
uniform vec3 viewPos;
uniform sampler2D texture_diffuse1;

void main()
{   
    // 环境光影计算
    vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords));
    
    // 漫反射光源
    vec3 norml = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);

    float diff = max(dot(norml, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * vec3(texture(texture_diffuse1, TexCoords)));

    // 镜面反射计算
    //float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norml);

    int specularFactor = 32;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), specularFactor);
    vec3 specular = light.specular * spec;

    vec3 result = ambient + diffuse + specular;

    FragColor = vec4(result, 1.0);
    //FragColor = texture(texture_diffuse1, TexCoords);
}