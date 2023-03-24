#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"
#include "stb_image/stb_image.h"

#include "core/shader.h"
#include "core/vertexBuffer.h"
#include "core/indexBuffer.h"
#include "core/vertexArray.h"
#include "core/renderer.h"
#include "core/log.h"
#include "core/texture.h"

#include "consts.h"

#include <iostream>

static void onWindowResized(GLFWwindow* window, int width, int height);
static void printDebugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
static bool initWindow(GLFWwindow** window);
static void initImGui(GLFWwindow* window);
static void update(float deltaTime);
static void render();

int main()
{
	GLFWwindow* window { nullptr };
	if (!initWindow(&window)) 
		return -1;
	initImGui(window);

	float lastFrameTime {};

	// ----- RENDER LOOP -----
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		float deltaTime = currentFrame - lastFrameTime;
		lastFrameTime = currentFrame;  		
		
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		
		ImGui::Begin("Application Menu");
		ImGui::SetWindowFontScale(Consts::IMGUI_UI_SCALE);
		ImGui::Text(("Frame time: " + std::to_string(deltaTime)).c_str());
		ImGui::End();
		
		update(deltaTime);
		render();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// ----- Cleanup -----
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate(); // this could cause an infinite loop if we were checking for errors in a loop with glGetError, 
	// because VAO goes out of scope (thus unbinds/deletes) after glfw terminates, so it will keep reporting this error. 
	return 0;
}

static void update(float deltaTime)
{
	// Update world, handle inputs
}

static void render()
{
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	// issue draw calls
}

static void onWindowResized(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

static void printDebugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	if (severity != GL_DEBUG_SEVERITY_NOTIFICATION)
	{
		Debug::log(std::string { message } + '\n');
	}
}

/// <returns>true if initialization was successful</returns>
static bool initWindow(GLFWwindow** window)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // opengl 4.3 for debug output
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	*window = glfwCreateWindow(Consts::SCREEN_WIDTH, Consts::SCREEN_HEIGHT, "First OpenGL Program", nullptr, nullptr);

	if (*window == nullptr)
	{
		Debug::log("Failed to create window\n", Debug::ERROR);
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(*window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		Debug::log("Failed to initialize GLAD\n", Debug::ERROR);
		return false;
	}

	std::cout << "openGL version: " << glGetString(GL_VERSION) << '\n';

	glEnable(GL_DEBUG_OUTPUT);
	//glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(printDebugMessage, NULL);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);

	glViewport(0, 0, Consts::SCREEN_WIDTH, Consts::SCREEN_HEIGHT);
	glfwSetWindowSizeCallback(*window, onWindowResized);
	glfwSwapInterval(0); // Enable vsync

	return true;
}

static void initImGui(GLFWwindow* window)
{
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 430");
}
