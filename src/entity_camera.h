#include "entity.h"

namespace kd
{

class Camera : public Entity
{
  public:
    explicit Camera();
    virtual ~Camera();

    virtual void Setup() override;
    virtual void Update(float deltaTime) override;

  protected:

  private:
    DISALLOW_COPY_AND_ASSIGN(Camera)
};

} // namespace kd