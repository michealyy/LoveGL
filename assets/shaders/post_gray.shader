#uniform

#vertex
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 texCoords;

void main()
{
    texCoords = aTexCoords;
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0); 
}

#fragment
#version 330 core
out vec4 FragColor;

in vec2 texCoords;

uniform sampler2D screenTexture;

void main()
{
    vec3 color = texture(screenTexture, texCoords).rgb;
    float a = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
    FragColor = vec4(a, a, a, 1.0);
}