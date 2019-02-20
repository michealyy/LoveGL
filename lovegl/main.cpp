#include "main.h"
#include <config.h>
#include <engine.h>

namespace kd
{

Main::Main()
{}

Main *Main::InitWindow(int width, int height)
{
    if (!glfwInit())
        return nullptr;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 8);
    //glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);

    window = glfwCreateWindow(width, height, "", nullptr, nullptr);
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

void Main::SetApp(App *app)
{
    Engine::GetInstance()->SetApp(app);

    Engine::GetInstance()->Setup();
}

void Main::MainLoop()
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