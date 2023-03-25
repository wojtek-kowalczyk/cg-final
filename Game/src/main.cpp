#include "consts.h"

#include "core/log.h"

#include "core/shader.h"
#include "core/vertexArray.h"
#include "core/vertexBuffer.h"
#include "core/indexBuffer.h"
#include "core/renderer.h"
#include "core/camera.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <iostream>

static void onWindowResized(GLFWwindow* window, int width, int height);
static void printDebugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
static bool initWindow(GLFWwindow** window);
static void initImGui(GLFWwindow* window);
static void update(GLFWwindow* window, float deltaTime, Camera& camera);
static void render(const Camera& camera);
static void imguiRender();
static void cleanupImGui();
static void cleanupWindow(GLFWwindow* window);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// TODO : this has to be global for callbacks?
Camera mainCamera;

int main()
{
	GLFWwindow* window;
	initWindow(&window);
	initImGui(window);

	mainCamera.Position = glm::vec3(3.0f, 3.0f, 3.0f);
	//mainCamera.Front = glm::vec3(0.0f, 0.0f, 0.0f) - mainCamera.Position;
	mainCamera.LookAt(glm::vec3(0.0f, 0.0f, 0.0f));

    while (!glfwWindowShouldClose(window))
    {
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		update(window, deltaTime, mainCamera);
		render(mainCamera);
		imguiRender();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

	cleanupImGui();
	cleanupWindow(window);
    return 0;
}

static void update(GLFWwindow* window, float deltaTime, Camera& camera) // TODO : I don't want to pass camera here. It should be updated like some kind of entity?
{
	// These are not handled in a callback because of the delay between key press and key repeat

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera::MovementDirection::Forward, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera::MovementDirection::Backward, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera::MovementDirection::Left, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera::MovementDirection::Right, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera::MovementDirection::Down, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera::MovementDirection::Up, deltaTime);
}

static void render(const Camera& camera)
{
	// Shader setup can be done outside. mind what things need to update (eg. view matrix coming from "camera")
	Shader shader{ "res/vert.glsl", "res/frag.glsl" };

	glm::mat4 model{ 1.0f };
	// If we don't use zoom this doesn't need to change every frame. Only when one of its settings changes.
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)Consts::SCREEN_WIDTH / (float)Consts::SCREEN_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();

	shader.use();
	shader.setMat4f("u_m", model);
	shader.setMat4f("u_v", view);
	shader.setMat4f("u_p", projection);

	float vertices[] = {
		// front			// front colors
		-1.0, -1.0, 1.0,	1.0, 1.0, 0.0,
		1.0, -1.0, 1.0,		0.0, 1.0, 0.0,
		1.0, 1.0, 1.0,		0.0, 0.0, 1.0,
		-1.0, 1.0, 1.0,		1.0, 1.0, 1.0,
		// back				// back colors
		-1.0, -1.0, -1.0,	0.0, 1.0, 1.0,
		1.0, -1.0, -1.0,	1.0, 0.0, 1.0,
		1.0, 1.0, -1.0,		1.0, 0.0, 0.0,
		-1.0, 1.0, -1.0, 	1.0, 1.0, 0.0
	};

	unsigned int indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		2, 1, 6,
		6, 1, 5,
		// back
		6, 7, 4,
		4, 5, 6,
		// left
		7, 4, 0,
		0, 7, 3,
		//top
		7, 3, 2,
		2, 6, 7,
		//bottom
		4, 0, 1,
		4, 1, 5, 
	};

	VertexArray vao;
	vao.Bind();

	VertexBufferLayout layout;
	layout.Add(VertexBufferLayout::Element{ 3, GL_FLOAT }); // position
	layout.Add(VertexBufferLayout::Element{ 3, GL_FLOAT }); // color
	VertexBuffer vbo{ vertices, sizeof(vertices) };

	IndexBuffer ibo{ indices, sizeof(indices) / sizeof(unsigned int) };

	vao.AddBuffer(vbo, layout);

	glClear(GL_DEPTH_BUFFER_BIT); // these are flags, so maybe pass clear flags to renderer from outside?

	Renderer rend;
	rend.Clear();
	rend.Draw(vao, ibo, shader);
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

	// TODO : some system where you have to click on the screen to lock mouse it and enable camera controls, press escape to unlock the cursor and disable camera controls
	glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSetWindowSizeCallback(*window, onWindowResized);
	glfwSetKeyCallback(*window, key_callback);
	glfwSetCursorPosCallback(*window, mouse_callback);
	glfwSetScrollCallback(*window, scroll_callback);
	glfwSwapInterval(0); // disable vsync , 1 - enable (I think?)

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		Debug::log("Failed to initialize GLAD\n", Debug::ERROR);
		return false;
	}

	// TODO : Make debug work with stream isnertion or some other easy way of formatting a string
	std::cout << "openGL version: " << glGetString(GL_VERSION) << '\n';
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEBUG_OUTPUT);
	//glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(printDebugMessage, NULL);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);

	glViewport(0, 0, Consts::SCREEN_WIDTH, Consts::SCREEN_HEIGHT);

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

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	static bool firstMouse = true;
	static float lastX, lastY;

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	mainCamera.ProcessMouseMovement(xoffset, yoffset); // how do I pass camera to this function, when I can't modify its interface?
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	mainCamera.ProcessMouseScroll(static_cast<float>(yoffset));
}