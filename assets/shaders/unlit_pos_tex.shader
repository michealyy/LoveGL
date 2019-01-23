#uniform

#vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 texCoord;

uniform mat4 mvp;

void main()
{
    gl_Position = mvp * vec4(aPos, 1.0);
    texCoord = aTexCoord;
}

#fragment
#version 330 core
out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D uTexture;
uniform vec3 color;
uniform float alpha;

void main()
{
    FragColor = vec4(1,0,0,1);//texture(uTexture, texCoord) * vec4(color, alpha);
} 