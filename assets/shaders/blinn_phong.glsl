#vertex
#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

out vec2 _texCoord;
out vec3 _normal;
out vec3 _fragPos;

uniform mat4 model;
uniform mat4 mvp;

void main()
{
    _texCoord = texCoord;
    _normal = normal;
    //世界坐标
    _fragPos = vec3(model * vec4(pos, 1.0));

    gl_Position = mvp * vec4(pos, 1.0);
}

#fragment
#version 330 core

out vec4 FragColor;

in vec2 _texCoord;
in vec3 _normal;
in vec3 _fragPos;

uniform sampler2D diffuseTexture;
//uniform float ambient;
//uniform float shininess;
uniform vec3 color;
uniform float alpha;
//主摄像机位置
uniform vec3 viewPos;

//平行光
struct DirectionalLight
{
    vec3 color;
    vec3 direction;
};  
uniform DirectionalLight directionalLight;

//点光源最大4个
#define MAX_POINT_LIGHTS 4
uniform int pointLightsCount;
struct PointLight
{
    vec3 color;
    vec3 position;
    float constant;
    float linear;
    float quadratic;
};
uniform PointLight pointLights[MAX_POINT_LIGHTS];

//聚光灯最大4个
#define MAX_SPOT_LIGHTS 4
uniform int spotLightsCount;
struct SpotLight
{
    vec3 color;
    vec3 position;
    vec3 direction;
    float innerAngle;
    float outerAngle;
};
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

//N法线 V视角向量 L片段到光源方向
float GetBlinnPhong(vec3 N, vec3 V, vec3 L)
{
    //uniform默认变量值
    float ambient = 0.1;
    float shininess = 100;
    
    //漫反射强度：平面法线和光源方向夹角相关，角度为0最大亮度1，数学表示cos角度
    float diffuse = clamp(dot(N, L), 0.0, 1.0);

    //镜面反射
    //Phong光源反射向量和视角向量相关+表面反光度
    //BlinnPhong法线和光源与视线中间向量
    // vec3 reflectLightDir = reflect(L, N);
    // float specular = pow(clamp(dot(V, normalize(reflectLightDir)), 0.0, 1.0), shininess);
    vec3 halfDir = normalize(L + V);
    float specular = pow(clamp(dot(N, halfDir), 0.0, 1.0), shininess);

    return (ambient + diffuse + specular);
}

vec3 GetDirectionalLight(DirectionalLight light, vec3 N, vec3 V)
{
    vec3 L = normalize(-light.direction);
    return light.color * GetBlinnPhong(N, V, L);
}

vec3 GetPointLight(PointLight light, vec3 N, vec3 V, vec3 fragPos)
{
    vec3 L = normalize(light.position - fragPos);

    //http://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    return light.color * GetBlinnPhong(N, V, L) * attenuation;
}

vec3 GetSpotLight(SpotLight light, vec3 N, vec3 V, vec3 fragPos)
{
    vec3 L = normalize(light.position - fragPos);
    
    float theta = dot(L, normalize(-light.direction));
    float epsilon = light.innerAngle - light.outerAngle;
    float intensity = clamp((theta - light.outerAngle) / epsilon, 0.0, 1.0);
    
    return light.color * GetBlinnPhong(N, V, L) * intensity;
}

void main()
{
    vec3 N = normalize(_normal);
    //vec3 N = normalize(vec3(1, 1, 1));
    //vec3 N = normalize(vec3(-1, 0, 0));
    vec3 V = normalize(viewPos - _fragPos);

    //平行光
    vec3 result = GetDirectionalLight(directionalLight, N, V);
    //点光
    for(int i = 0; i < clamp(pointLightsCount, 0, MAX_POINT_LIGHTS); i++)
    {
        result += GetPointLight(pointLights[i], N, V, _fragPos);
    }
    //聚光
    for(int i = 0; i < clamp(spotLightsCount, 0, MAX_SPOT_LIGHTS); i++)
    {
        result += GetSpotLight(spotLights[i], N, V, _fragPos);
    }

    FragColor = vec4(texture(diffuseTexture, _texCoord).rgb * result, 1.0);// * vec4(color, alpha);
    
    //[HDR] 曝光色调映射
    // float exposure = 1.0;
    // vec3 hdrColor = texture(diffuseTexture, _texCoord).rgb * result;
    // vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
    // FragColor = vec4(mapped, 1.0);
}