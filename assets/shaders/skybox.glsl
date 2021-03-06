#vertex
#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 texCoord;

uniform mat4 mvp;

void main()
{
    texCoord = aPos;
    gl_Position = mvp * vec4(aPos, 1.0);
}

#fragment
#version 330 core
out vec4 FragColor;

in vec3 texCoord;

uniform samplerCube skybox;

void main()
{
    FragColor = textureLod(skybox, texCoord, 0);
} 