#vertex
#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

out vec2 _texCoord;
out vec3 _normal;
out vec3 _worldPos;

uniform mat4 model;
uniform mat4 mvp;

void main()
{
    _texCoord = texCoord;
    _normal = mat3(model) * normal;
    _worldPos = vec3(model * vec4(pos, 1.0));

    gl_Position = mvp * vec4(pos, 1.0);
}

#fragment
#version 330 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec2 _texCoord;
in vec3 _normal;
in vec3 _worldPos;

//IBL信息
uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D brdfLUT;

uniform vec3 viewPos;
uniform float alpha;

uniform vec3 albedo;
uniform float roughness;
uniform float metallic;
uniform sampler2D albedoTexture;
uniform sampler2D metallicRoughnessTexture;

//平行光
struct DirectionalLight
{
    vec3 color;
    vec3 direction;
};  
uniform DirectionalLight directionalLight;

//点光
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

const float PI = 3.14159265359;

//正态分布函数Normal Distribution Function，表示与中间向量一致的微平面的比率
//Trowbridge-Reitz GGX
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

//菲涅尔方程Fresnel Rquation，表示不同角度反射光线所占的比率
//Fresnel-Schlick Approximation
vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}
//IBL不是单一光线，无法算出中间向量，替代为受粗糙度影响的方程
vec3 FresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
} 

//粗糙度来估算微平面对某个向量遮蔽
float GeometrySchlickGGX(float NdotV, float roughness)
{
    //IBL不一样，粗糙度重映射
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
//几何函数Geometry Function，表示微平面间相互遮蔽的比率
//Smith’s Schlick-GGX
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    //观察方向 几何遮蔽(Geometry Obstruction)
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    //光线方向 几何阴影(Geometry Shadowing)
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

//Cook-Torrance BRDF
vec3 BRDF(vec3 N, vec3 H, vec3 V, vec3 L, vec3 F0, vec3 albedo, float roughness)
{
    //镜面反射
    float D = DistributionGGX(N, H, roughness);   
    float G = GeometrySmith(N, V, L, roughness);      
    vec3 F = FresnelSchlick(max(dot(H, V), 0.0), F0);

    float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
    vec3 specular = D * G * F / max(denominator, 0.0000001);

    //漫反射，根据能量守恒，可通过镜面反射比例来求
    vec3 kS = F; //F代表就是镜面反射比例
    vec3 kD = vec3(1.0) - kS;
    
    kD *= 1.0 - metallic; //金属度越高，漫反射比例越少

    vec3 lambertian = albedo / PI; //Lambertian漫反射

    //漫反射*漫反射比例+镜面反射*镜面反射比例
    return kD * lambertian + specular;
}

void main()
{
    vec3 albedo = albedo + texture(albedoTexture, _texCoord).rgb;
    vec4 mrt_ = texture(metallicRoughnessTexture, _texCoord);
    float roughness = roughness + mrt_.g;
    float metallic = metallic + mrt_.b;
    
    vec3 V = normalize(viewPos - _worldPos);
    vec3 N = normalize(_normal);
    vec3 R = reflect(-V, N);

    //金属度越高，基础反射率高
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);
    
    //#多光源
    // vec3 L = normalize(pointLights[0].position - _worldPos);
    // vec3 H = normalize(V + L);
    // float distance = length(pointLights[0].position - _worldPos);
    // float attenuation = 1.0 / (distance * distance);
    // vec3 radiance = pointLights[0].color * attenuation;
    vec3 L = normalize(-directionalLight.direction);
    vec3 H = normalize(V + L);
    //辐射度
    vec3 radiance = directionalLight.color * 4;
    //#多光源

    //辐射率和光线和法线夹角相关
    float NdotL = max(dot(N, L), 0.0);

    //#IBL
    vec3 F = FresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
    
    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;	  
    
    vec3 irradiance = texture(irradianceMap, N).rgb;
    vec3 diffuse = irradiance * albedo;
    
    const float MAX_REFLECTION_LOD = 4;
    vec3 prefilteredColor = textureLod(prefilterMap, R, roughness * MAX_REFLECTION_LOD).rgb;    
    vec2 brdf  = texture(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
    vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);

    vec3 ambient = (kD * diffuse + specular);
    //#IBL

    //环境光固定0.1+反射光
    //vec3 result = vec3(0.1) * albedo + BRDF(N, H, V, L, F0) * radiance * NdotL;
    vec3 result = ambient + BRDF(N, H, V, L, F0, albedo, roughness) * radiance * NdotL;
    
    // result = result / (result + vec3(1.0));
    //result = pow(result, vec3(1.0/2.2));

    float brightness = dot(result, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        BrightColor = vec4(result, 1.0);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);

    FragColor = vec4(result, alpha);
}