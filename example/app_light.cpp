﻿#include "app_light.h"
#include <glm/glm.hpp>

using namespace kd;
using namespace glm;

AppLight::AppLight()
{
}

AppLight::~AppLight()
{
}

void AppLight::Setup()
{
    scnMgr = new SceneManager();
    Engine::GetInstance()->sceneManager = scnMgr;

    scnMgr->LoadGLTF("assets/scenes/1.gltf");
    //scnMgr->LoadGLTF("assets/tftest/tftest.gltf");

    //主摄像机
    // auto camera = scnMgr->GetCamera("Camera_Orientation");
    // camera->AttachController(new FreeCameraController());
    // //camera->AttachPostProcessing(new PostGray());
    // Engine::GetInstance()->mainCamera = camera;

    auto camera = scnMgr->CreateNode<Camera>();
    camera->name = "MainCamera";
    camera->AttachController(new FreeCameraController());
    camera->position = vec3(0, 0, 10);
    Engine::GetInstance()->mainCamera = camera;

    //灯源
    auto directionalLight = scnMgr->CreateNode<DirectionalLight>();
    directionalLight->color = vec3(0.5, 0.5, 0.5);
    //directionalLight->color = vec3(0, 0, 0);
    directionalLight->direction = vec3(0, -1, 0);

    auto pointLight = scnMgr->CreateNode<PointLight>();
    pointLight->color = vec3(1, 0, 0);
    pointLight->position = vec3(-2, 1, 0);
    pointLight->linear = 0.14f;
    pointLight->quadratic = 0.07f;

    auto pointLight2 = scnMgr->CreateNode<PointLight>();
    pointLight2->color = vec3(0, 1, 0);
    pointLight2->position = vec3(0, 1, -5);
    pointLight2->linear = 0.14f;
    pointLight2->quadratic = 0.07f;

    auto spotLight = scnMgr->CreateNode<PointLight>();
    spotLight->color = vec3(0, 0, 1);
    spotLight->position = vec3(0, 1, 5);
    // spotLight->direction = vec3(0, -1, 0);
    // spotLight->innerAngle = cos(radians(12.5f));
    // spotLight->outerAngle = cos(radians(17.5f));
}

void AppLight::Update(float deltaTime)
{
    //场景拾取
    auto main_window = Engine::GetInstance()->GetMainWindow();
    double x, y;
    glfwGetCursorPos(main_window, &x, &y);

    //主摄像机鼠标点转换到世界空间射线
    auto ray = Engine::GetInstance()->mainCamera->MousePointToRay(vec2(x, y));
    //测试屏幕射线是否正确
    auto p1 = ray.GetPoint(1);
    float line[] = {p1.x, p1.y, p1.z, 0, 0, 0};
    Renderer::GetInstance()->DrawDebugLine(line);

    if (glfwGetMouseButton(main_window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS && Engine::GetInstance()->mainCamera)
    {
        for (auto nodes : scnMgr->GetNodes())
        {
            auto mesh = dynamic_cast<Mesh *>(nodes);
            if (mesh)
            {
                RayCastHit rayCastHit;
                if (mesh->Raycast(ray, rayCastHit))
                {
                    Engine::GetInstance()->selected_node = rayCastHit.node;
                    break;
                }
            }
        }
    }
}
