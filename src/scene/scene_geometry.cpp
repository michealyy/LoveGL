#include "scene_geometry.h"
#include <glm/glm.hpp>
#include "../engine.h"
#include "../material.h"
#include "../entity_camera.h"
#include "../entity_box.h"
#include "../entity_cylinder.h"
#include "../entity_cone.h"
#include "../ui/ui_rect.h"

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
    camera->canController = true;
    camera->position = vec3(0, 1, 5);
    Engine::GetInstance()->mainCamera = camera;
    AddEntity(camera);

    auto mat1 = new Material("unlit_pos_1");
    mat1->SetShader("unlit_pos");
    mat1->SetColor(vec3(1, 0, 0));
    mat1->SetAlpha(0.2f);
    auto box1 = new Box(mat1, 1, 1, 1);
    box1->scale = vec3(2, 2, 0.2);
    box1->eulerAngles = vec3(0, 0, 45);
    box1->name = "box1";
    root->AddChild(box1);

    auto mat2 = new Material("unlit_pos_2");
    mat2->SetShader("unlit_pos");
    mat2->SetColor(vec3(0, 1, 0));
    mat2->SetAlpha(1);
    auto cone1 = new Cone(mat2);
    cone1->color = vec3(0, 1, 0);
    cone1->name = "cone1";
    cone1->position = vec3(-2, 0, 0);
    root->AddChild(cone1);

    auto mat3 = new Material("unlit_pos_tex_1");
    mat3->SetShader("unlit_pos_tex");
    mat3->SetTexture(0, "wood");
    auto box2 = new Box(mat3, 1, 1, 1);
    box2->name = "box2";
    box2->position = vec3(0, 0, -2);
    root->AddChild(box2);

    auto mat4 = new Material("unlit_pos_3");
    mat4->SetShader("unlit_pos");
    mat4->SetColor(vec3(0, 0, 1));
    mat4->SetAlpha(0.2f);
    auto box3 = new Box(mat4, 1, 1, 1);
    box3->name = "box3";
    box3->position = vec3(0, 0, 2);
    root->AddChild(box3);

    Scene::Setup();
}

void SceneGeometry::Update(float deltaTime)
{
    Scene::Update(deltaTime);
}

} // namespace kd