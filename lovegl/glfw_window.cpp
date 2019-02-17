#include "glfw_window.h"
#include <config.h>
#include <engine.h>

namespace kd
{

GLFWWindow::GLFWWindow()
{}

GLFWWindow *GLFWWindow::InitWindow(int width, int height, const char *title)
{
    if (!glfwInit())
        return nullptr;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 8);
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);

    GLenum glewErr = glewInit();
    if (GLEW_OK != glewErr)
    {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(glewErr));
        glfwTerminate();
        return nullptr;
    }

    if (Vsync)
    {
        glfwSwapInterval(1);
    }
    else
    {
        glfwSwapInterval(0);
    }

    //glEnable(GL_FRAMEBUFFER_SRGB);

    Engine::GetInstance()->SetMainWindow(window);

    return this;
}

void GLFWWindow::SetApp(App *app)
{
    Engine::GetInstance()->SetApp(app);

    Engine::GetInstance()->Setup();
}

void GLFWWindow::MainLoop()
{
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(CLEAR_COLOR.r, CLEAR_COLOR.g, CLEAR_COLOR.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Engine::GetInstance()->Update();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}

} // namespace kd