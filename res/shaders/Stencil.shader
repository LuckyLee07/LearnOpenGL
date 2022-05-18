#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float scaleFactor;

void main()
{
    //gl_Position = projection * view * model * vec4(aPos, 1.0);
    //先算出观察坐标系法线，然后相对各个坐标系，更新各坐标系需要的位置，加上系数乘法线
    vec3 Normal = mat3(transpose(inverse(model))) * aNormal;
    gl_Position = projection * view * model * vec4(aPos+Normal*scaleFactor, 1.0);
}

#shader fragment
#version 330 core

out vec4 FragColor;

void main()
{   
    FragColor = vec4(0.04, 0.28, 0.26, 1.0);
}