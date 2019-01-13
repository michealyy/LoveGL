#uniform

#vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec4 color;

uniform float a;
uniform mat4 mvp;

void main()
{
    gl_Position = mvp * vec4(aPos, 1.0);
    color = vec4(aColor, 1.0);
}

#fragment
#version 330 core
out vec4 FragColor;

in vec4 color;

void main()
{
    FragColor = color;
}