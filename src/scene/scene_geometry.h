#pragma once

#include <vector>
#include "../common.h"
#include "../scene.h"

namespace kd
{

class SceneGeometry : public Scene
{
  public:
    explicit SceneGeometry();
    virtual ~SceneGeometry();

    virtual void Setup() override;
    virtual void Update(float deltaTime) override;

  private:

    DISALLOW_COPY_AND_ASSIGN(SceneGeometry)
};

} // namespace kd