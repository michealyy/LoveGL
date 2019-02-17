#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "app_light.h"

int main(int, char **)
{
	using namespace kd;

	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 8);
	glfwWindowHint(GLFW_SCALE_TO_MONITOR,  GLFW_TRUE);

	GLFWwindow *window = glfwCreateWindow(static_cast<int>(WINDOW_SIZE.x), static_cast<int>(WINDOW_SIZE.y), "LoveGL-keddy", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

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
	Engine::GetInstance()->SetApp(new AppLight());
	
	Engine::GetInstance()->Setup();
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(CLEAR_COLOR.r, CLEAR_COLOR.g, CLEAR_COLOR.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Engine::GetInstance()->Update();

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
