#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "common.h"
#include "app.h"

namespace kd
{

class GLFWWindow final
{
public:
  GLFWWindow();
  GLFWWindow *InitWindow(int width, int height, const char *title);
  void SetApp(App *app);
  void MainLoop();

  GLFWwindow *window = nullptr;

private:
  DISALLOW_COPY_AND_ASSIGN(GLFWWindow)
};

} // namespace kd
