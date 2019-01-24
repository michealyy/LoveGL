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

//UIRect不添加到场景，添加到ui_root下面。不要使用scale，直接设置高宽即可，影响事件点击。
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
  UIRect *FindChild(float x, float y);

  float width = 0.0f;
  float height = 0.0f;
  glm::vec3 color{1};
  float alpha = 1.0f;

  Material *material = nullptr;

private:
  DISALLOW_COPY_AND_ASSIGN(UIRect)
};

} // namespace ui

} // namespace kd