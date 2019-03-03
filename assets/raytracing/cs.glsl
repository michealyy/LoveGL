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
};

struct Box
{
    vec3 min;
    vec3 max;
};

struct BoxHitInfo
{
    float near;
    int i;
};

#define LARGE_FLOAT 1E+10
#define NUM_BOXES 1
const Box boxes[] =
{
  {vec3(-0.5, 0.0, -0.5), vec3(0.5, 1.0, 0.5)},
  {vec3(-5.0, -0.1, -5.0), vec3(5.0, 0.0, 5.0)}
};

vec2 intersectBox(vec3 origin, vec3 dir, const Box b)
{
    vec3 tMin = (b.min - origin) / dir;
    vec3 tMax = (b.max - origin) / dir;
    vec3 t1 = min(tMin, tMax);
    vec3 t2 = max(tMin, tMax);
    float tNear = max(max(t1.x, t1.y), t1.z);
    float tFar = min(min(t2.x, t2.y), t2.z);
    return vec2(tNear, tFar);
}

bool intersectBoxes(in Ray ray, out BoxHitInfo info)
{
    float smallest = LARGE_FLOAT;
    bool found = false;
    for (int i = 0; i < NUM_BOXES; i++) 
    {
        vec2 lambda = intersectBox(ray.origin, ray.direction, boxes[i]);
        if (lambda.y >= 0.0 && lambda.x < lambda.y && lambda.x < smallest) 
        {
            info.near = lambda.x;
            info.i = i;
            smallest = lambda.x;
            found = true;
        }
    }
    return found;
}

vec4 Hit(in BoxHitInfo info)
{
    //Box box = boxes[info.i];
    if (info.i == 0)
        return vec4(1, 0 ,0, 1);
    if (info.i == 1)
        return vec4(0, 1 ,0, 1);
}

vec4 Miss()
{
    return vec4(0, 0, 0, 1);
}

vec4 TraceRay(in Ray ray)
{
    BoxHitInfo boxHitAttr;
    if (!intersectBoxes(ray, boxHitAttr))
        return Miss();
    else
      return Hit(boxHitAttr);
}

/*
*   计算单元和屏幕像素一一对应。屏幕每点即当前计算单元生成一条射线
*   射线起点为摄像机世界位置，向量为屏幕点转空间摄像机空间坐标
*/
void GenerateCameraRay(out Ray ray)
{
    ivec2 pos = ivec2(gl_GlobalInvocationID.xy);
    ivec2 size = imageSize(RenderTarget);

    //vec2 screenPos = vec2(pos) / vec2(size);
    vec2 screenPos = (vec2(pos) + vec2(0.5)) / vec2(size);
    vec4 worldPos = unProject * vec4(screenPos, 1, 1);
    vec3 direction = vec3(worldPos.xyz);

    ray.origin = cameraPosition;
    ray.direction = direction;
}

void main() 
{
    Ray ray;
    GenerateCameraRay(ray);
    vec4 hdrColor = TraceRay(ray);

    ivec2 pos = ivec2(gl_GlobalInvocationID.xy);
    imageStore(RenderTarget, pos, hdrColor);
    //imageStore(RenderTarget, pos, vec4(screenPos, 0, 1));
}