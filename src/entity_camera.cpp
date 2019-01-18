#include "entity_camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <GLFW/glfw3.h>
#include "engine.h"

using namespace glm;

namespace kd
{

Camera::Camera()
{
    SetPerspective();
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

    if (canController)
        UpdateControl(deltaTime);
}

void Camera::SetPerspective()
{
    isOrtho = false;

    int width, height;
    glfwGetWindowSize(Engine::GetInstance()->GetMainWindow(), &width, &height);

    projectMatrix = perspective(radians(fov), (float)width / (float)height, _near, _far);
}

void Camera::SetOrtho(vec2 size)
{
    isOrtho = true;
    projectMatrix = ortho(left, right, bottom, top, _near, _far);
}

mat4 Camera::GetViewMatrix()
{
    return inverse(worldTransform);
}

void Camera::UpdateControl(float deltaTime)
{
    auto window = Engine::GetInstance()->GetMainWindow();

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) != GLFW_PRESS)
    {
        firstMousePos = true;
        return;
    }

    if (!firstMousePos)
    {
        double mousePosX, mousePosY;
        glfwGetCursorPos(window, &mousePosX, &mousePosY);
        float x = (float)(mousePosY - lastMousePosY);
        float y = (float)(mousePosX - lastMousePosX);
        eulerAngles += vec3(x * rotateSpeed * deltaTime, y * rotateSpeed * deltaTime, 0);
        
        if (glfwGetKey(window, GLFW_KEY_W))
        {
            auto movedelta = worldTransform * vec4(0, 0, -1, 0) * moveSpeed;
            position = position + vec3(movedelta.x, movedelta.y, movedelta.z);
        }
        if (glfwGetKey(window, GLFW_KEY_S))
        {
            auto movedelta = worldTransform * vec4(0, 0, 1, 0) * moveSpeed;
            position = position + vec3(movedelta.x, movedelta.y, movedelta.z);
        }
        if (glfwGetKey(window, GLFW_KEY_A))
        {
            auto movedelta = worldTransform * vec4(-1, 0, 0, 0) * moveSpeed;
            position = position + vec3(movedelta.x, movedelta.y, movedelta.z);
        }
        if (glfwGetKey(window, GLFW_KEY_D))
        {
            auto movedelta = worldTransform * vec4(1, 0, 0, 0) * moveSpeed;
            position = position + vec3(movedelta.x, movedelta.y, movedelta.z);
        }
    }
    else
    {
        firstMousePos = false;
    }

    glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);
}

} // namespace kd