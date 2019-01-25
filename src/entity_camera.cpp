#include "entity_camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <GLFW/glfw3.h>
#include "engine.h"

using namespace glm;

namespace kd
{

CameraController::CameraController()
{
}

CameraController::~CameraController()
{
}

Camera::Camera()
{
    SetPerspective();
}

Camera::~Camera()
{
    SafeDelete(cameraController);
}

void Camera::Setup()
{
    Entity::Setup();
    if (cameraController)
        cameraController->Setup();
}

void Camera::Update(float deltaTime)
{
    Entity::Update(deltaTime);

    if (cameraController)
        cameraController->Update(deltaTime);
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

Ray Camera::ScreenPointToRay(vec2 point)
{
    int width, height;
    glfwGetWindowSize(Engine::GetInstance()->GetMainWindow(), &width, &height);
    
    auto world_dir = normalize(unProject(vec3(point, 1), worldTransform, projectMatrix, vec4(0.0f, 0.0f, width, height)));

    // // float mouseX = getMousePositionX() / (getWindowWidth()  * 0.5f) - 1.0f;
    // // float mouseY = getMousePositionY() / (getWindowHeight() * 0.5f) - 1.0f;

    // // glm::mat4 proj = glm::perspective(FoV, AspectRatio, Near, Far);
    // // glm::mat4 view = glm::lookAt(glm::vec3(0.0f), CameraDirection, CameraUpVector);

    // glm::mat4 invVP = glm::inverse(projectMatrix * worldTransform);
    // //glm::vec4 screenPos = glm::vec4(mouseX, -mouseY, 1.0f, 1.0f);
    // glm::vec4 worldPos = invVP * screenPos;

    // glm::vec3 world_dir = glm::normalize(glm::vec3(worldPos));

    return Ray{worldPosition, world_dir};
}

FreeCameraController::FreeCameraController()
{
}

FreeCameraController::~FreeCameraController()
{
}

void FreeCameraController::Setup()
{
}

void FreeCameraController::Update(float deltaTime)
{
    if (camera == nullptr)
        return;

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
        camera->eulerAngles += vec3(x * rotateSpeed * deltaTime, y * rotateSpeed * deltaTime, 0);

        if (glfwGetKey(window, GLFW_KEY_W))
        {
            auto movedelta = camera->worldTransform * vec4(0, 0, -1, 0) * moveSpeed;
            camera->position = camera->position + vec3(movedelta.x, movedelta.y, movedelta.z);
        }
        if (glfwGetKey(window, GLFW_KEY_S))
        {
            auto movedelta = camera->worldTransform * vec4(0, 0, 1, 0) * moveSpeed;
            camera->position = camera->position + vec3(movedelta.x, movedelta.y, movedelta.z);
        }
        if (glfwGetKey(window, GLFW_KEY_A))
        {
            auto movedelta = camera->worldTransform * vec4(-1, 0, 0, 0) * moveSpeed;
            camera->position = camera->position + vec3(movedelta.x, movedelta.y, movedelta.z);
        }
        if (glfwGetKey(window, GLFW_KEY_D))
        {
            auto movedelta = camera->worldTransform * vec4(1, 0, 0, 0) * moveSpeed;
            camera->position = camera->position + vec3(movedelta.x, movedelta.y, movedelta.z);
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE))
        {
            auto movedelta = camera->worldTransform * vec4(0, 1, 0, 0) * moveSpeed;
            camera->position = camera->position + vec3(movedelta.x, movedelta.y, movedelta.z);
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
        {
            auto movedelta = camera->worldTransform * vec4(0, -1, 0, 0) * moveSpeed;
            camera->position = camera->position + vec3(movedelta.x, movedelta.y, movedelta.z);
        }
    }
    else
    {
        firstMousePos = false;
    }

    glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);
}

} // namespace kd