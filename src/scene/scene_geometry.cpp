#include "scene_geometry.h"
#include <glm/glm.hpp>
#include "../engine.h"
#include "../material.h"
#include "../entity_camera.h"
#include "../entity_box.h"
#include "../entity_cylinder.h"
#include "../entity_cone.h"

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
    auto camera = new Camera();
    camera->name = "SceneGeometry camera";
    camera->canController = true;
    camera->position = glm::vec3(1.5f, 0, 5);
    Engine::GetInstance()->mainCamera = camera;
    AddEntity(camera);

    auto mat1 = new Material("test");
    mat1->SetShader("test");

    auto box1 = new Box(mat1, 1, 1, 1, vec3(1, 0, 0));
    box1->name = "box1";
    box1->position = glm::vec3(1.5f, 0, -2);
    // box1->scale = glm::vec3(0.5f, 0.5f, 1);
    box1->eulerAngles = glm::vec3(0, 0, 45);
    AddEntity(box1);

    auto box2 = new Box(mat1, 1.f, 1.f, 1, vec3(0, 1, 0));
    box2->name = "box2";
    //box2->position = glm::vec3(0.5f, 0, 0);
    box2->eulerAngles = glm::vec3(0, 0, 60);
    //box1->AddChild(box2);

    // auto box3 = new Box(mat1, 1, 1, 1, vec3(0, 0, 1));
    // box3->name = "box3";
    // box3->position = glm::vec3(1.f, 0, 0);
    // box3->scale = glm::vec3(0.5f, 0.5f, 1);
    // box3->eulerAngles = glm::vec3(0, 0, 90);
    // AddEntity(box3);


    // auto cylinder1 = new Cylinder(mat1);
    // cylinder1->color = vec3(0, 1, 0);
    // cylinder1->name = "cylinder1";
    // cylinder1->position = glm::vec3(1.5f, 0, 0);
    // AddEntity(cylinder1);

    auto cone1 = new Cone(mat1);
    cone1->color = vec3(0, 1, 0);
    cone1->name = "cone1";
    cone1->position = glm::vec3(1.5f, 0, 0);
    AddEntity(cone1);

    Scene::Setup();
}

void SceneGeometry::Update(float deltaTime)
{
    Scene::Update(deltaTime);
}

} // namespace kd