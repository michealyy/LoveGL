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
out vec4 fragColor;
in vec2 texCoords;
uniform sampler2D image;
void main()
{
    fragColor = texture(image, texCoords);
}