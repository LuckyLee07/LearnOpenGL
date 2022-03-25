#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;

out  vec3 ourColor;
out  vec2 texCoord;

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

layout(location = 0) out vec4 color;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{   
    vec4 texColor = texture(u_Texture, texCoord);
    vec4 tempColor = vec4(u_Color.r, ourColor.g, u_Color.b, 1.0);
    color = mix(texColor, tempColor, 0.2);
}