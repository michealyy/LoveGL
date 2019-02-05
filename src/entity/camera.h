#pragma once

#include <node.h>
#include <physics/ray.h>
#include <core/render_target.h>
#include <core/post_processing.h>

namespace kd
{

class CameraController
{
public:
  explicit CameraController();
  virtual ~CameraController();
  virtual void Setup() = 0;
  virtual void Update(float deltaTime) = 0;
  Node *camera = nullptr;

private:
  DISALLOW_COPY_AND_ASSIGN(CameraController)
};

class Camera : public Node
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
    inline void AttachController(CameraController *controller)
    {
      cameraController = controller;
      controller->camera = this;
    };
    inline void AttachPostProcessing(PostProcessing *postProcessing)
    {
      this->postProcessing = postProcessing;
      postProcessing->camera = this;
    };
    
    PostProcessing *postProcessing = nullptr;
    RenderTarget *renderTarget = nullptr;
    int depth = 0;
    bool isOverlay = true;
    glm::vec3 clearColor{1};
    bool isLinePolygon = false;
    glm::mat4 projectMatrix{1.0f};

  private:
    CameraController *cameraController = nullptr;

    float fov = 45.0f;
    bool isOrtho = false;
    float _near = 0.1f;
    float _far = 1000.0f;
    float left = 0.0f;
    float right = 1.0f;
    float bottom = 0.0f;
    float top = 1.0f;

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