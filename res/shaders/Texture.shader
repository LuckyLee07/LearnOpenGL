#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;

out  vec3 ourColor;
out  vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    ourColor = aColor;
    texCoord = aTexCoord;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}

#shader fragment
#version 330 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 texCoord;

uniform sampler2D u_Texture1;
uniform sampler2D u_Texture2;

void main()
{   
    vec4 texColor1 = texture(u_Texture1, texCoord);
    vec4 texColor2 = texture(u_Texture2, texCoord);

    FragColor = mix(texColor1, texColor2, 0.95);
    //FragColor = texColor1 * vec4(ourColor, 1.0f);
}