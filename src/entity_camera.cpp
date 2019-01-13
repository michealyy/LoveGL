#include "entity_camera.h"

namespace kd {
Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::Setup()
{
    Entity::Setup();
}

void Camera::Update(float deltaTime)
{
    Entity::Update(deltaTime);
}

}