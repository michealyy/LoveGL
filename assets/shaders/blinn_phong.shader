#uniform
shininess float 100

#vertex
#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

out vec2 _texCoord;
out vec3 _normal;
out vec3 _lightDir;
out vec3 _cameraDir;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPosition;
uniform vec3 cameraPos;

void main()
{
    vec4 worldPos = model * vec4(pos, 1.0);

    _texCoord = texCoord;
    _normal = normal;
    _lightDir = lightPosition - worldPos.xyz;
    _cameraDir = cameraPos - worldPos.xyz;

    gl_Position = projection * view * worldPos;
}

#fragment
#version 330 core

out vec4 FragColor;

in vec2 _texCoord;
in vec3 _normal;
in vec3 _lightDir;
in vec3 _cameraDir;

uniform sampler2D mainTexture;
uniform float shininess;

void main()
{
    vec3 L = normalize(_lightDir);
    vec3 V = normalize(_cameraDir);
    vec3 N = normalize(_normal);

    float ambient = 0.2;
    //float shininess = 100;
    //漫反射强度：平面法线和光源方向夹角相关
    float diffuse = clamp(dot(N, L), 0.0, 1.0);

    //镜面反射
    //Phong光源反射向量和视角向量相关+表面反光度
    //BlinnPhong法线和光源与视线中间向量
    // vec3 reflectLightDir = reflect(L, N);
    // float specular = pow(clamp(dot(V, normalize(reflectLightDir)), 0.0, 1.0), shininess);
    vec3 halfDir = normalize(L + V);
    float specular = pow(clamp(dot(N, halfDir), 0.0, 1.0), shininess);

    vec3 finalColor = texture(mainTexture, _texCoord).rgb * (ambient + diffuse + specular);

    FragColor = vec4(finalColor, 1.0);
}