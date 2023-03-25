#include "consts.h"

#include "core/log.h"

#include "core/shader.h"
#include "core/vertexArray.h"
#include "core/vertexBuffer.h"
#include "core/indexBuffer.h"
#include "core/renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <iostream>

static void onWindowResized(GLFWwindow* window, int width, int height);
static void printDebugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
static bool initWindow(GLFWwindow** window);
static void initImGui(GLFWwindow* window);
static void render();
static void imguiRender();
static void cleanupImGui();
static void cleanupWindow(GLFWwindow* window);

int main()
{
	GLFWwindow* window;
	initWindow(&window);
	initImGui(window);

    while (!glfwWindowShouldClose(window))
    {
		render();
		imguiRender();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

	cleanupImGui();
	cleanupWindow(window);
    return 0;
}

static void cleanupWindow(GLFWwindow* window)
{

	glfwDestroyWindow(window);
	glfwTerminate();
}

static void cleanupImGui()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

static void imguiRender()
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("hello dear imgui");
	ImGui::Text("Some Text");
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

static void render()
{
	Shader shader{ "res/vert.glsl", "res/frag.glsl" };

	float vertices[] = {
	 -0.5f, -0.5f, 0.0f,
	  0.5f, -0.5f, 0.0f,
	  0.5f,  0.5f, 0.0f,
	 -0.5f,  0.5f, 0.0f,
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0,
	};

	VertexArray vao;
	vao.Bind();

	VertexBufferLayout layout;
	layout.Add(VertexBufferLayout::Element{ 3, GL_FLOAT });
	VertexBuffer vbo{ vertices, sizeof(vertices) };

	IndexBuffer ibo{ indices, sizeof(indices) / sizeof(unsigned int) };

	vao.AddBuffer(vbo, layout);

	Renderer rend;
	rend.Clear();
	rend.Draw(vao, ibo, shader);
}

static void onWindowResized(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

static void printDebugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	//if (severity != GL_DEBUG_SEVERITY_NOTIFICATION)
	{
		//Debug::log(std::string { message } + '\n', type == GL_DEBUG_TYPE_ERROR? Debug::ERROR : Debug::INFO);

		//fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
			//(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
			//type, severity, message);

		// https://learnopengl.com/In-Practice/Debugging
		// ignore non-significant error/warning codes
		if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

		std::cout << "---------------" << std::endl;
		std::cout << "Debug message (" << id << "): " << message << std::endl;

		switch (source)
		{
			case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
			case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
			case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
			case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
			case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
			case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
		} std::cout << std::endl;

		switch (type)
		{
			case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
			case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
			case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
			case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
			case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
			case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
			case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
		} std::cout << std::endl;

		switch (severity)
		{
			case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
			case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
			case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
			case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
		} std::cout << std::endl;
		std::cout << std::endl;
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

	*window = glfwCreateWindow(Consts::SCREEN_WIDTH, Consts::SCREEN_HEIGHT, "CG Final", nullptr, nullptr);

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

	// TODO : Make debug work with stream isnertion or some other easy way of formatting a string
	std::cout << "openGL version: " << glGetString(GL_VERSION) << '\n';

	glEnable(GL_DEBUG_OUTPUT);
	//glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(printDebugMessage, NULL);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);

	glViewport(0, 0, Consts::SCREEN_WIDTH, Consts::SCREEN_HEIGHT);
	glfwSetWindowSizeCallback(*window, onWindowResized);
	glfwSwapInterval(0); // disable vsync , 1 - enable (I think?)

	return true;
}

static void initImGui(GLFWwindow* window)
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	//ImGuiIO& io = ImGui::GetIO(); 
	//(void)io; // why does the example do this?
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 430");
}
