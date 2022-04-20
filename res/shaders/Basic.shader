#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;

out  vec3 ourColor;
out  vec2 texCoord;

//uniform mat4 u_MVP;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    ourColor = aColor;
    texCoord = aTexCoord;
}

#shader fragment
#version 330 core

in vec3 ourColor;
in vec2 texCoord;

out vec4 outColor;

uniform sampler2D u_Texture;
//uniform vec4 u_Color;

void main()
{   
    vec4 texColor = texture(u_Texture, texCoord);
    outColor = texColor * vec4(ourColor, 1.0f);
    outColor = texColor;
}