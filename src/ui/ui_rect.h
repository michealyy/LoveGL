#pragma once

#include <string>
#include <vector>
#include "../common.h"
#include "../entity.h"
#include "../material.h"

namespace kd
{

namespace ui
{

//UIRect不添加到场景，添加到ui_root下面
class UIRect : public Entity
{
public:
  explicit UIRect(float width = 100.f, float height = 100.f);
  virtual ~UIRect();
  //方便创建材质用。如果要设置不同color，alpha需要创建新材质
  void SetImage(const std::string &name);

  virtual void Setup() override;
  virtual void Update(float deltaTime) override;
  virtual void OnMouseLeftButtonPress();
  virtual void OnMouseLeftButtonRelease();
  virtual void OnMouseHover();
  //实现鼠标事件用
  UIRect *FindRect(float x, float y);

  float width = 0.0f;
  float height = 0.0f;
  int depth = 0;
  glm::vec3 color{1};
  float alpha = 1.0f;

  glm::vec3 right_bottom;
  glm::vec3 right_top;
  glm::vec3 left_top;
  glm::vec3 left_bottom;
  glm::vec2 uv_right_bottom{1, 1};
  glm::vec2 uv_right_top{1, 0};
  glm::vec2 uv_left_top{0, 0};
  glm::vec2 uv_left_bottom{0, 1};

  Material *material = nullptr;

private:
  DISALLOW_COPY_AND_ASSIGN(UIRect)
};

} // namespace ui

} // namespace kd