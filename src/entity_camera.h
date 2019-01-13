#pragma once

#include "entity.h"

namespace kd
{

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

  float fov = 45.0f;
  bool isOrtho = false;
  float _near = 0.1f;
  float _far = 1000.0f;
  float left = 0.0f;
  float right = 1.0f;
  float bottom = 0.0f;
  float top = 1.0f;

  glm::mat4 projectMatrix{1.0f};

  bool canController = false;
  float moveSpeed = 4.0f;
  float rotateSpeed = 0.1f;

protected:
  virtual void UpdateControl(float deltaTime);
  bool firstMousePos = true;
  double lastMousePosX = 0;
  double lastMousePosY = 0;

private:
  DISALLOW_COPY_AND_ASSIGN(Camera)
};

} // namespace kd