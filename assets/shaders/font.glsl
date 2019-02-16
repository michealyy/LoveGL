#vertex
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec4 aColor;

out vec2 texCoord;
out vec4 color;

uniform mat4 mvp;

void main()
{
    gl_Position = mvp * vec4(aPos, 1.0, 1.0);
    texCoord = aTexCoord;
    color = aColor;
}

#fragment
#version 330 core
out vec4 FragColor;

in vec2 texCoord;
in vec4 color;

uniform sampler2D uTexture;

void main()
{
    FragColor = vec4(1.0, 1.0, 1.0, texture(uTexture, texCoord).r);// * color;
} 