#version 430 core
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
};

struct SphereHitInfo
{
    vec3 point;
    vec3 normal;
};

/*
*   球标准方程，射线某点存在球上
*/
float intersectSphere(vec3 origin, vec3 direction, vec3 center, float radius)
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

void Hit(inout Payload payload, in SphereHitInfo info)
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

void TraceRay(in Sphere[3] scene, in Ray ray, inout Payload payload)
{
    bool isHit = false;
    
    for (int i = 0; i < scene.length(); i++)
    {
        float t = intersectSphere(ray.origin, ray.direction, scene[i].center, scene[i].radius);
        if (t > 0)
        {
            isHit = true;

            SphereHitInfo sphereHitInfo;
            sphereHitInfo.point = ray.origin + t * ray.direction;
            sphereHitInfo.normal = normalize(sphereHitInfo.point - scene[i].center);
            Hit(payload, sphereHitInfo);
        }
    }

    if (!isHit)
        Miss(payload);
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
    spheres[0].center = vec3(0, -100.5, -1);
    spheres[0].radius = 100;
    spheres[1].center = vec3(-0.8, 0, 0);
    spheres[1].radius = 0.5;
    spheres[2].center = vec3(0.8, 0, 0);
    spheres[2].radius = 0.5;
    TraceRay(spheres, ray, payload);
    imageStore(RenderTarget, ivec2(gl_GlobalInvocationID.xy), payload.color);
}

//https://github.com/noobdawn/RayTracingWithU3DEditor/blob/master/SimpleRayTracing.cs