#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "common.h"
#include "app.h"

namespace kd
{

class Main final
{
public:
  Main();
  Main *InitWindow(int width, int height, const char *title);
  void SetApp(App *app);
  void MainLoop();

  GLFWwindow *window = nullptr;

private:
  DISALLOW_COPY_AND_ASSIGN(Main)
};

} // namespace kd
