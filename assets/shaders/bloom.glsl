#vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos, 1.0);
}

#fragment
#version 440 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D image;
uniform sampler2D blur;
uniform float factor;
uniform float exposure;

void main()
{             
    vec3 hdrColor = texture(image, TexCoords).rgb;      
    vec3 bloomColor = texture(blur, TexCoords).rgb;

    hdrColor += bloomColor * factor;

    //tone mapping
    vec3 result = vec3(1.0) - exp(-hdrColor * exposure);

    //gamma correct 
    result = pow(result, vec3(1.0 / 2.2));

    FragColor = vec4(result, 1.0);
}