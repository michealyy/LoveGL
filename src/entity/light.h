#pragma once

#include <glm/glm.hpp>
#include <node.h>

namespace kd
{

class Light : public Node
{
public:
  explicit Light();
  virtual ~Light();
  glm::vec3 color{1};

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
  float linear = 0.045f;
  float quadratic = 0.0075f;

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
  float innerAngle = 0.f;
  float outerAngle = 0.f;

private:
  DISALLOW_COPY_AND_ASSIGN(SpotLight)
};

} // namespace kd