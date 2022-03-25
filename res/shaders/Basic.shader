#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

out  vec3 ourColor;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    ourColor = aColor;
}

#shader fragment
#version 330 core

in vec3 ourColor;

layout(location = 0) out vec4 color;

uniform vec4 u_Color;

void main()
{   
    //color = u_Color;
    color = vec4(u_Color.r, ourColor.g, u_Color.b, 1.0);
}