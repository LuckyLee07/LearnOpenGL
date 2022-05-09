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
// 定向光
struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
// 点光源
struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};
// 聚光（手电筒）
struct SpotLight {
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    float cutOff;
    float outerCutOff;
};

struct Material {
    //vec3 diffuse;
    //vec3 specular;
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 viewPos;
uniform Material material;

uniform DirLight dirLight;
#define NUM_POINT_LIGHTS 4
uniform PointLight pointLights[NUM_POINT_LIGHTS];
uniform SpotLight spotLight;

// 定向光的计算方式
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
// 点光源的计算方法
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
// 聚光(手电筒)的计算方式
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{   
    // 一些属性计算
    vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // 第一阶段：定向光照
    vec3 result = CalcDirLight(dirLight, normal, viewDir);

    // 第二阶段：点光源
    for(int i = 0; i < NUM_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], normal, FragPos, viewDir);

    // 第三阶段：聚光手电筒
    result += CalcSpotLight(spotLight, normal, FragPos, viewDir);

    FragColor = vec4(result, 1.0);
}

// 定向光的计算方式
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    // 从片段指向光源的方向 定向光为方向向量的反方向
    vec3 lightDir = normalize(-light.direction);

    //环境光着色
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    
    // 漫反射光着色
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse, TexCoords)));

    // 镜面反射光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);    
    vec3 specular = light.specular * (spec * vec3(texture(material.specular, TexCoords)));

    return (ambient + diffuse + specular);
}

// 点光源的计算方法
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    // 从片段指向光源的方向
    vec3 lightDir = normalize(light.position - fragPos);

    //环境光着色
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    
    // 漫反射光着色
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse, TexCoords)));

    // 镜面反射光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);    
    vec3 specular = light.specular * (spec * vec3(texture(material.specular, TexCoords)));

    // 点光源衰减计算
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    ambient *= attenuation; diffuse *= attenuation; specular *= attenuation;

    return (ambient + diffuse + specular);
}

// 聚光(手电筒)的计算方式
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    // 从片段指向光源的方向
    vec3 lightDir = normalize(light.position - fragPos);

    //环境光着色
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    
    // 漫反射光着色
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse, TexCoords)));

    // 镜面反射光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);    
    vec3 specular = light.specular * (spec * vec3(texture(material.specular, TexCoords)));

    // 点光源衰减计算
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    ambient *= attenuation; diffuse *= attenuation; specular *= attenuation;

    // 聚光计算 判断是否在圆锥角内
    float theta = dot(lightDir, normalize(-light.direction));

    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // 对环境光不做影响 让它总是能有一点儿光
    diffuse *= intensity; specular *= intensity;

    return (ambient + diffuse + specular);
}