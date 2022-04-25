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

in vec3 ourColor;
in vec2 texCoord;

out vec4 outColor;

uniform sampler2D u_Texture1;
uniform sampler2D u_Texture2;

void main()
{   
    vec4 texColor1 = texture(u_Texture1, texCoord);
    vec4 texColor2 = texture(u_Texture2, texCoord);
    //outColor = texColor1 * vec4(ourColor, 1.0f);
    outColor = mix(texColor1, texColor2, 0.2);
    //outColor = texture(u_Texture2, vec2(1.0-texCoord.x, texCoord.y));
}