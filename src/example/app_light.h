#pragma once

#include "../common.h"
#include "../app.h"
#include "../scene_manager.h"

class AppLight : public kd::App
{
public:
  explicit AppLight();
  virtual ~AppLight();
  virtual void Setup() override;
  virtual void Update(float deltaTime) override;

  kd::SceneManager *scnMgr = nullptr;

private:
  DISALLOW_COPY_AND_ASSIGN(AppLight)
};
