#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "config.h"
#include "engine.h"

int main(int, char**) {
	using namespace kd;

	if (!glfwInit())
		return -1;

	// glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	// glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	// glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(static_cast<int>(WINDOW_SIZE.x), static_cast<int>(WINDOW_SIZE.y), "kdRenderStudio", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (LINE_Polygon)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	
	Engine::GetInstance()->SetMainWindow(window);
	Engine::GetInstance()->OnSetup();
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(UI_CLEAR_COLOR.r, UI_CLEAR_COLOR.g, UI_CLEAR_COLOR.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		Engine::GetInstance()->OnUpdate();

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
