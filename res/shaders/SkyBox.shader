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

uniform samplerCube skybox;

void main()
{    
    FragColor = texture(skybox, TexCoords);
}