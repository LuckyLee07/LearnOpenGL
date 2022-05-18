#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    TexCoords = aTexCoords;
}

#shader fragment
#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

const float offset = 1.0 / 300.0;

void main()
{ 
    vec2 offsets[9] = vec2[](
        vec2( -offset,  offset),
        vec2(  0.0f,    offset),
        vec2(  offset,  offset),
        vec2( -offset,  0.0f),
        vec2(  0.0f,    0.0f),
        vec2(  offset,  0.0f),
        vec2( -offset, -offset),
        vec2(  0.0f,   -offset),
        vec2(  offset, -offset)
    );
    //克尔效应 卷积矩阵
    float kernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );
    //模糊效果 卷积矩阵
    float kernel1[9] = float[](
        1.0/16, 2.0/16, 1.0/16,
        2.0/16, 4.0/16, 2.0/16,
        1.0/16, 2.0/16, 1.0/16
    );
    //锐化效果 边缘检测
    float kernel2[9] = float[](
        1,  1,  1,
        1, -8,  1,
        1,  1,  1
    );
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }
    vec3 color = vec3(0.0);
    for(int i = 0; i < 9; i++)
        color += sampleTex[i] * kernel2[i];
    
    FragColor = vec4(color, 1.0);
}