#pragma once

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

protected:

private:
  DISALLOW_COPY_AND_ASSIGN(Light)
};

class PointLight : public Light
{
public:
  explicit PointLight();
  virtual ~PointLight();

private:
  DISALLOW_COPY_AND_ASSIGN(PointLight)
};

class SpotLight : public Light
{
public:
  explicit SpotLight();
  virtual ~SpotLight();

private:
  DISALLOW_COPY_AND_ASSIGN(SpotLight)
};

} // namespace kd