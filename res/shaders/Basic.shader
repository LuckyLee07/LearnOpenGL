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

uniform sampler2D u_Texture1;
uniform sampler2D u_Texture2;
//uniform vec4 u_Color;
uniform float u_ratio;

void main()
{   
    vec4 texColor1 = texture(u_Texture1, texCoord);
    vec4 texColor2 = texture(u_Texture2, texCoord);
    //outColor = texColor1 * vec4(ourColor, 1.0f);
    outColor = mix(texColor1, texColor2, u_ratio);
    //outColor = texture(u_Texture2, vec2(1.0-texCoord.x, texCoord.y));
}