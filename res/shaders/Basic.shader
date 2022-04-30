#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
//layout(location = 1) in vec3 aColor;
layout(location = 1) in vec2 aTexCoord;

out  vec3 ourColor;
out  vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    ourColor = vec3(1.0, 1.0, 1.0);
    texCoord = aTexCoord;
}

#shader fragment
#version 330 core

out vec4 outColor;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{   
    outColor = vec4(lightColor * objectColor, 1.0);
}