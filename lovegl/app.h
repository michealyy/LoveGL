#pragma once

#include "common.h"

namespace kd
{

class App
{
public:
  explicit App();
  virtual ~App();
  virtual void Setup();
  virtual void Update(float deltaTime);

private:
  DISALLOW_COPY_AND_ASSIGN(App)
};

} // namespace kd