#version 430 core
#define PI 3.14159265359
layout (local_size_x = 32, local_size_y = 32) in;

layout(binding = 0, rgba32f) uniform image2D RenderTarget;
uniform vec3 cameraPosition;
uniform mat4 unProject;

struct Ray
{
    vec3 origin;
    vec3 direction;
};

struct Payload
{
    vec4 color;
    vec2 pos;
};

struct Sphere
{
    vec3 center;
    float radius;
    vec3 color;
};

struct SphereHitInfo
{
    int index;
    vec3 point;
    vec3 normal;
};

/*
*  射线相交于球。射线上存某点在球上，带入球标准方程
*/
float IntersectSphere(vec3 origin, vec3 direction, vec3 center, float radius)
{
    vec3 oc = origin - center;
    float a = dot(direction, direction);
    float b = 2 * dot(oc, direction);
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0)
        return -1;
    else
        return (-b - sqrt(discriminant)) / (2 * a);
}

void AnyHit(inout Payload payload, in SphereHitInfo info)
{
    //可视化法线
    vec3 n = info.normal;
    payload.color = 0.5 * vec4(n.x + 1, n.y + 1, n.z + 1, 1);

    //测试光源
    vec3 lightColor = vec3(1, 0, 0);
    vec3 lightDir = normalize(vec3(0, 1, 0));
    float NdotL = max(dot(info.normal, lightDir), 0.0);
    payload.color = vec4(lightColor * NdotL, 1);
}

void Miss(inout Payload payload)
{
    //淡蓝色渐变背景
    payload.color = mix(vec4(1, 1, 1, 1), vec4(0.5, 0.7, 1, 1), payload.pos.y);
}

uniform int MaxRecursionDepth = 3;
void TraceRay(in Sphere[3] scene, in Ray _ray, inout Payload payload)
{
    Ray ray = _ray;
    vec3 attenuation = vec3(1);
    //把递归改成了循环
    for (int depth = 0; depth < MaxRecursionDepth; depth++)
    {
        bool isHit = false;
        
        SphereHitInfo sphereHitInfo;
        for (int i = 0; i < scene.length(); i++)
        {
            float t = IntersectSphere(ray.origin, ray.direction, scene[i].center, scene[i].radius);
            if (t > 0)
            {
                isHit = true;
                sphereHitInfo.index = i;
                sphereHitInfo.point = ray.origin + t * ray.direction;
                sphereHitInfo.normal = normalize(sphereHitInfo.point - scene[i].center);
                //AnyHit(payload, sphereHitInfo);
            }
        }
        
        if (isHit)
        {
            // attenuation *= dot(ray.direction, sphereHitInfo.normal);
            attenuation *= scene[sphereHitInfo.index].color;

            //取当前反射射线，下次循环使用
            ray.origin = sphereHitInfo.point;
            ray.direction = reflect(ray.direction, sphereHitInfo.normal);
            
            //ClosestHit
        }
        else
        {
            payload.color = mix(vec4(1, 1, 1, 1), vec4(0.5, 0.7, 1, 1), payload.pos.y) * vec4(attenuation, 1);
            //Miss(payload);
            return;
        }
    }
    payload.color = vec4(0.1, 0.1, 0.1, 1);
}

/*
*   计算单元和屏幕像素一一对应。屏幕每点即当前计算单元生成一条以视点为起点的射线
*/
void GenerateCameraRay(out Ray ray, out Payload payload)
{
    ivec2 pos = ivec2(gl_GlobalInvocationID.xy);
    ivec2 size = imageSize(RenderTarget);

    //vec2 screenPos = vec2(pos) / vec2(size);
    vec2 screenPos = (vec2(pos) + vec2(0.5)) / vec2(size);
    vec4 worldPos = unProject * vec4(screenPos, 1, 1);
    vec3 direction = vec3(worldPos.xyz);

    payload.pos = screenPos;
    ray.origin = cameraPosition;
    ray.direction = normalize(direction);
}

void main() 
{
    Ray ray;
    Payload payload;
    GenerateCameraRay(ray, payload);

    Sphere spheres[3];
    spheres[0].color = vec3(0.5, 0.5, 0.5);
    spheres[0].center = vec3(0, -100.5, -1);
    spheres[0].radius = 100;
    spheres[1].color = vec3(0.8, 0, 0.4);
    spheres[1].center = vec3(-0.8, 0, 0);
    spheres[1].radius = 0.5;
    spheres[2].color = vec3(0.8, 0.4, 0);
    spheres[2].center = vec3(0.8, 0, 0);
    spheres[2].radius = 0.5;
    TraceRay(spheres, ray, payload);
    imageStore(RenderTarget, ivec2(gl_GlobalInvocationID.xy), payload.color);
}

//https://github.com/noobdawn/RayTracingWithU3DEditor/blob/master/SimpleRayTracing.cs