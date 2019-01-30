﻿#include "app_light.h"
#include <glm/glm.hpp>
#include "../engine.h"
#include "../renderer.h"

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
    
    //主摄像机
    auto camera = scnMgr->CreateEntity<Camera>();
    camera->name = "MainCamera";
    camera->SetController(new FreeCameraController());
    camera->position = vec3(0, 0, 10);
    Engine::GetInstance()->mainCamera = camera;

    //灯源
    auto directionalLight = scnMgr->CreateEntity<DirectionalLight>();
    directionalLight->color = vec3(0.4, 0.4, 0.4);
    directionalLight->direction = vec3(0, -1, 0);

    auto pointLight = scnMgr->CreateEntity<PointLight>();
    pointLight->color = vec3(1, 0, 0);
    pointLight->position = vec3(-2, 1, 0);
    pointLight->linear = 0.14f;
    pointLight->quadratic = 0.07f;
    
    auto pointLight2 = scnMgr->CreateEntity<PointLight>();
    pointLight2->color = vec3(0, 0, 1);
    pointLight2->position = vec3(0, 1, -5);
    pointLight2->linear = 0.14f;
    pointLight2->quadratic = 0.07f;

    auto spotLight = scnMgr->CreateEntity<SpotLight>();
    spotLight->color = vec3(0, 1, 0);
    spotLight->position = vec3(2, 2, 2);
    spotLight->direction = vec3(0, -1, 0);
    spotLight->angle = cos(radians(15.f));

    //网格物体
    auto mat1 = new Material("unlit_pos_1");
    mat1->SetShader("unlit_pos");
    mat1->SetColor(vec3(1, 0, 0));
    mat1->SetAlpha(0.5f);
    auto cylinder1 = scnMgr->CreateEntity<Mesh>();
    cylinder1->material = mat1;
    cylinder1->SetMesh("cylinder");
    cylinder1->name = "cylinder1";
    cylinder1->position = vec3(-3, 0, 0);
    
    auto mat2 = new Material("unlit_pos_2");
    mat2->SetShader("unlit_pos");
    mat2->SetColor(vec3(0, 1, 0));
    mat2->SetAlpha(0.2f);
    auto cone1 = scnMgr->CreateEntity<Mesh>();
    cone1->material = mat2;
    cone1->SetMesh("cone");
    cone1->name = "cone1";
    cone1->position = vec3(0, 0, 0);

    auto mat3 = new Material("unlit_pos_tex_1");
    mat3->SetShader("unlit_pos_tex");
    mat3->SetTexture("wood");
    auto box1 = scnMgr->CreateEntity<Mesh>();
    box1->material = mat3;
    box1->SetMesh("box");
    box1->name = "box1";
    box1->position = vec3(3, 0, 0);

    auto mat4 = new Material("blinn_phong_1");
    mat4->SetShader("blinn_phong");
    mat4->SetTexture("white");
    mat4->SetColor(vec3(1, 0, 0));
    auto sphere = scnMgr->CreateEntity<Mesh>();
    sphere->material = mat4;
    sphere->SetMesh("sphere");
    sphere->name = "sphere";
    sphere->position = vec3(6, 0, 0);

    //大地面
    auto mat5 = new Material("blinn_phong_2");
    mat5->SetShader("blinn_phong");
    mat5->SetTexture("white");
    auto plane = scnMgr->CreateEntity<Mesh>();
    plane->material = mat5;
    plane->SetMesh("plane");
    plane->name = "plane";
    plane->position = vec3(0, -1, 0);
    plane->scale = vec3(10, 1, 10);
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
    float line[] = {p1.x, p1.y, p1.z, 0,0,0};
    Renderer::GetInstance()->DrawDebugLine(line);

    if (glfwGetMouseButton(main_window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS && Engine::GetInstance()->mainCamera)
    {
        for(auto entities : scnMgr->GetEntities())
        {
            auto mesh = dynamic_cast<Mesh*>(entities);
            if (mesh)
            {
                RayCastHit rayCastHit;
                if (mesh->Raycast(ray, rayCastHit))
                {
                    Engine::GetInstance()->selected_entity = rayCastHit.entity;
                    break;
                }
            }
        }
    }
}