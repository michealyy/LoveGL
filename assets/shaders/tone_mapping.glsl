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
uniform float exposure;
uniform float gamma = 2.2;

vec3 Uncharted2Tonemap(vec3 x)
{
    const float A = 0.15f;
    const float B = 0.50f;
    const float C = 0.10f;
    const float D = 0.20f;
    const float E = 0.02f;
    const float F = 0.30f;

    return ((x * (A * x + C * B) + D * E) / (x * (A * x + B) + D * F)) - E / F;
}

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
    vec3 hdrColor = texture(image, texCoords).rgb;    
    
    //tone mapping
    vec3 result = ACESToneMapping(hdrColor, exposure);
    //vec3 result = vec3(1.0) - exp(-hdrColor * exposure);

    //gamma correct 
    result = pow(result, vec3(1.0 / gamma));
    
    fragColor = vec4(result, 1.0);
}