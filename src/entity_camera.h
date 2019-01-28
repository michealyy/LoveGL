#pragma once

#include "entity.h"
#include "ray.h"

namespace kd
{

class CameraController
{
public:
  explicit CameraController();
  virtual ~CameraController();
  virtual void Setup() = 0;
  virtual void Update(float deltaTime) = 0;
  Entity *camera = nullptr;

private:
  DISALLOW_COPY_AND_ASSIGN(CameraController)
};

class Camera : public Entity
{
public:
  explicit Camera();
  virtual ~Camera();

  virtual void Setup() override;
  virtual void Update(float deltaTime) override;

  void SetPerspective();
  void SetOrtho(glm::vec2 size);
  glm::mat4 GetViewMatrix();
  Ray MousePointToRay(glm::vec2 point);
  inline void SetController(CameraController *controller)
  {
    cameraController = controller;
    controller->camera = this;
  };

  float fov = 45.0f;
  bool isOrtho = false;
  float _near = 0.1f;
  float _far = 1000.0f;
  float left = 0.0f;
  float right = 1.0f;
  float bottom = 0.0f;
  float top = 1.0f;

  glm::mat4 projectMatrix{1.0f};

private:
  CameraController *cameraController = nullptr;

  DISALLOW_COPY_AND_ASSIGN(Camera)
};

class FreeCameraController : public CameraController
{
public:
  explicit FreeCameraController();
  virtual ~FreeCameraController();
  virtual void Setup() override;
  virtual void Update(float deltaTime) override;

  float moveSpeed = 0.1f;
  float rotateSpeed = 10.f;

private:
  bool firstMousePos = true;
  double lastMousePosX = 0;
  double lastMousePosY = 0;
  DISALLOW_COPY_AND_ASSIGN(FreeCameraController)
};

} // namespace kd