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
uniform float gamma = 2.2;

vec3 ACESToneMapping(vec3 color, float adapted_lum)
{
	const float A = 2.51f;
	const float B = 0.03f;
	const float C = 2.43f;
	const float D = 0.59f;
	const float E = 0.14f;

	color *= adapted_lum;
	return (color * (A * color + B)) / (color * (C * color + D) + E);
}

void main()
{             
    vec3 hdrColor = texture(image, TexCoords).rgb;      
    vec3 bloomColor = texture(blur, TexCoords).rgb;

    hdrColor += bloomColor * factor;
    
    vec3 result = ACESToneMapping(hdrColor, exposure);
    result = pow(result, vec3(1.0 / gamma));
    FragColor = vec4(result, 1.0);
}