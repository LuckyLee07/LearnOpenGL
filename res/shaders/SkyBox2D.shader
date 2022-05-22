#shader vertex
#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = aPos; //当立方体中央处于原点时 立方体上顶点位置即等价于采样向量
    vec4 pos = projection * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww; // z -> w/w = 1.0
}

#shader fragment
#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform sampler2D posx;
uniform sampler2D negx;
uniform sampler2D posy;
uniform sampler2D negy;
uniform sampler2D posz;
uniform sampler2D negz;

vec4 textureCube(sampler2D posx, sampler2D negx, sampler2D posy, sampler2D negy, sampler2D posz, sampler2D negz, vec3 texCoords);

void main()
{   
    FragColor = textureCube(posx, negx, posy, negy, posz, negz, TexCoords);
}

//由于我们立方体的坐标范围是[−1, 1]，那么把它转换到[0, 1]只需一个转换即可：(x + 1) / 2;
vec4 textureCube(sampler2D posx, sampler2D negx, sampler2D posy, sampler2D negy, sampler2D posz, sampler2D negz, vec3 texCoords)
{
    float mag = max(max(abs(texCoords.x), abs(texCoords.y)), abs(texCoords.z));
    if (mag == abs(texCoords.x)) // 采样x面
    {
        float texPosx = (texCoords.z + 1) / 2;
        float texPosy = (texCoords.y + 1) / 2;
        if (texCoords.x > 0.0)
            return texture(posx, vec2(1 - texPosx, texPosy));
        else if (texCoords.x < 0.0)
            return texture(negx, vec2(texPosx, texPosy));
    }
    else if (mag == abs(texCoords.y)) // 采样y面
    {
        float texPosx = (texCoords.x + 1) / 2;
        float texPosy = (texCoords.z + 1) / 2;
        if (texCoords.y > 0.0)
            return texture(posy, vec2(texPosx, 1 - texPosy));
        else if (texCoords.y < 0.0)
            return texture(negy, vec2(texPosx, texPosy));
    }
    else if (mag == abs(texCoords.z)) // 采样z面
    {
        float texPosx = (texCoords.x + 1) / 2;
        float texPosy = (texCoords.y + 1) / 2;
        if (texCoords.z > 0.0)
            return texture(posz, vec2(texPosx, texPosy));
        else if (texCoords.z < 0.0)
            return texture(negz, vec2(1 - texPosx, texPosy));
    }
}