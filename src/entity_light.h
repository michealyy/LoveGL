#pragma once

#include <glm/glm.hpp>
#include "entity.h"

namespace kd
{

class Light : public Entity
{
public:
  explicit Light();
  virtual ~Light();

  virtual void Setup() override;
  virtual void Update(float deltaTime) override;
  glm::vec3 color{1};

protected:

private:
  DISALLOW_COPY_AND_ASSIGN(Light)
};

class DirectionalLight : public Light
{
public:
  explicit DirectionalLight();
  virtual ~DirectionalLight();
  glm::vec3 direction{1};

private:
  DISALLOW_COPY_AND_ASSIGN(DirectionalLight)
};

//http://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation
class PointLight : public Light
{
public:
  explicit PointLight();
  virtual ~PointLight();
  float constant = 1.f;
  float linear;
  float quadratic;

private:
  DISALLOW_COPY_AND_ASSIGN(PointLight)
};

class SpotLight : public Light
{
public:
  explicit SpotLight();
  virtual ~SpotLight();
  glm::vec3 position;
  glm::vec3 direction;
  float angle;

private:
  DISALLOW_COPY_AND_ASSIGN(SpotLight)
};

} // namespace kd