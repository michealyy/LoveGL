#pragma once

#include <string>
#include <vector>
#include "../common.h"
#include "../entity.h"
#include "../material.h"

namespace kd
{

class UIRect : public Entity
{
public:
  explicit UIRect(float width = 100.f, float height= 100.f);
  virtual ~UIRect();

  virtual void Update(float deltaTime) override;
  //void SetImage(const std::string& name);

  float width = 0.0f;
  float height = 0.0f;
  glm::vec3 color{1};
  float alpha = 1.0f;
  unsigned short depth = 0;

  Material *material = nullptr;

private:
  DISALLOW_COPY_AND_ASSIGN(UIRect)
};

} // namespace kd