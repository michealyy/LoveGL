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
    return inverse(mat4_cast(quat(this->eulerAngles))) * translate(mat4(1.0f), this->position);
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
        float x = (float)(lastMousePosY - mousePosY);
        float y = (float)(lastMousePosX - mousePosX);
        eulerAngles += vec3(x * rotateSpeed * deltaTime, y * rotateSpeed * deltaTime, 0);

        //mat4 modelMatrix = transform->GetModelMatrix();
        //#include <glm/gtc/matrix_access.hpp>
        //column(modelMatrix, 0);
        if (glfwGetKey(window, GLFW_KEY_W))
        {
        }
        if (glfwGetKey(window, GLFW_KEY_S))
        {
        }
        if (glfwGetKey(window, GLFW_KEY_A))
        {
        }
        if (glfwGetKey(window, GLFW_KEY_D))
        {
        }
    }
    else
    {
        firstMousePos = false;
    }

    glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);
}

} // namespace kd