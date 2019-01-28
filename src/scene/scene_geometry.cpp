#include "scene_geometry.h"
#include <glm/glm.hpp>
#include "../engine.h"
#include "../renderer.h"
#include "../material.h"
#include "../entity_camera.h"
#include "../entity_geometry.h"
#include "../ui/ui_rect.h"
#include "../ui/ui_root.h"

using namespace glm;

namespace kd
{

SceneGeometry::SceneGeometry()
{
}

SceneGeometry::~SceneGeometry()
{
}

void SceneGeometry::Setup()
{
    auto root = new Entity();
    root->name = "Root";
    AddEntity(root);

    auto camera = new Camera();
    camera->name = "SceneGeometry camera";
    camera->SetController(new FreeCameraController());
    camera->position = vec3(0, 0, 10);
    Engine::GetInstance()->mainCamera = camera;
    AddEntity(camera);

    auto mat1 = new Material("unlit_pos_1");
    mat1->SetShader("unlit_pos");
    mat1->SetColor(vec3(1, 0, 0));
    mat1->SetAlpha(0.5f);
    auto cylinder1 = new Mesh(mat1);
    cylinder1->SetMesh("cylinder");
    cylinder1->name = "cylinder1";
    cylinder1->position = vec3(-3, 0, 0);
    root->AddChild(cylinder1);

    auto mat2 = new Material("unlit_pos_2");
    mat2->SetShader("unlit_pos");
    mat2->SetColor(vec3(0, 1, 0));
    auto cone1 = new Mesh(mat2);
    cone1->SetMesh("cone");
    cone1->name = "cone1";
    cone1->position = vec3(0, 0, 0);
    root->AddChild(cone1);

    auto mat3 = new Material("unlit_pos_tex_1");
    mat3->SetShader("unlit_pos_tex");
    mat3->SetTexture("wood");
    auto box1 = new Mesh(mat3);
    box1->SetMesh("box");
    box1->name = "box1";
    box1->position = vec3(3, 0, 0);
    root->AddChild(box1);

    auto mat4 = new Material("unlit_pos_3");
    mat4->SetShader("unlit_pos");
    mat4->SetColor(vec3(0, 0, 1));
    mat4->SetAlpha(0.2f);
    auto sphere = new Mesh(mat4);
    sphere->SetMesh("sphere");
    sphere->name = "sphere";
    sphere->position = vec3(6, 0, 0);
    root->AddChild(sphere);

    Scene::Setup();
}

void SceneGeometry::Update(float deltaTime)
{
    Scene::Update(deltaTime);

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
        for(auto entities : entities_)
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

} // namespace kd