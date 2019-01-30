#include "scene_manager.h"
#include "engine.h"

using namespace std;

namespace kd
{

SceneManager::SceneManager()
{
    root_ = new Entity();
    root_->name = "Root";
    entities_.push_back(root_);
}

SceneManager::~SceneManager()
{
    for (auto entity : entities_)
    {
        SafeDelete(entity);
    }
}

void SceneManager::Setup()
{
    for (auto entity : entities_)
    {
        entity->Setup();
    }
}

void SceneManager::Update(float deltaTime)
{
    if (Engine::GetInstance()->mainCamera == nullptr)
    {
        fprintf(stderr, "not find mainCamera. pls set Engine::GetInstance()->mainCamera!");
    }

    for (auto entity : entities_)
    {
        if (entity->visible)
            entity->Update(deltaTime);
    }
}

} // namespace kd