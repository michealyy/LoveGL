#include "scene.h"
#include "engine.h"

using namespace std;

namespace kd
{

Scene::Scene()
{
}

Scene::~Scene()
{
    for (auto entity : entities_)
    {
        if (!entity->notDeleteByScene)
            SafeDelete(entity);
    }
}

void Scene::Setup()
{
    for (auto entity : entities_)
    {
        entity->Setup();
    }
}

void Scene::Update(float deltaTime)
{
    if (Engine::GetInstance()->mainCamera == nullptr)
    {
        fprintf(stderr, "not find mainCamera. pls set Engine::GetInstance()->mainCamera!");
    }

    for (auto entity : entities_)
    {
        entity->Update(deltaTime);
    }
}

} // namespace kd