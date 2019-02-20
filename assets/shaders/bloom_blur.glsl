#vertex
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0); 
}

#fragment
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D image;

uniform bool horizontal;
uniform float weight[11] = float[] (0.035822, 0.05879, 0.086425, 0.113806, 0.13424, 0.141836, 0.13424, 0.113806, 0.086425, 0.05879, 0.035822);

void main()
{             
    vec3 result = vec3(0.0f, 0.0f, 0.0f);
    vec2 pixelSize = 1.0f / textureSize(image, 0);

    if(horizontal)
    {
        for (int i = -5; i <= 5; ++i)
        {
            result += texture(image, TexCoords + vec2(pixelSize.x * i, 0.0)).rgb * weight[i + 5];
        }
    }
    else
    {
        for (int i = -5; i <= 5; ++i)
        {
            result += texture(image, TexCoords + vec2(0.0, pixelSize.y * i)).rgb * weight[i + 5];
        }
    }
    FragColor = vec4(result, 1.0);
}