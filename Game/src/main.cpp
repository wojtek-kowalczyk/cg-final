#include "consts.h"

#include "core/log.h"

#include "core/shader.h"
#include "core/vertexArray.h"
#include "core/vertexBuffer.h"
#include "core/indexBuffer.h"
#include "core/renderer.h"
#include "core/camera.h"
#include "core/mesh.h"
#include "core/object.h"	
#include "core/primitives.h"
#include "core/time.h"
#include "core/scene.h"
#include "core/texture.h"
#include "core/loader.h"

#include "core/behaviours/testBehaviour.h"

#include "core/shaders.h" // TODO : remove this from core? does it even matter?

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <iostream>

// TODO handle single and multi mech objects as parts of the same object

// links to models:
// https://free3d.com/3d-model/abandoned-cottage-house-825251.htmlstatic 
// https://www.kenney.nl/assets/survival-kit

void onWindowResized(GLFWwindow* window, int width, int height);
static void printDebugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
static bool initWindow(GLFWwindow** window);
static void initImGui(GLFWwindow* window);
static void imguiRender(Scene& scene, float deltaTime);
static void cleanupImGui();
static void cleanupWindow(GLFWwindow* window);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
static void setupScene(Scene& scene);
static void setupDirectionalLight(Scene& scene);
static void setupSpotlights(Scene& scene);
static void setupPointLights(Scene& scene);

// TODO : maybe put the camera entirely in a scene?
// TODO : this has to be global for callbacks? Figure out a way to not do that
// TODO : watch a video on smart pointers (vs regualr pointers, mind global camera shared ptr. make it unique? what would that mean?)
std::shared_ptr<Camera> mainCamera;

int main()
{
	GLFWwindow* window;
	initWindow(&window);
	initImGui(window);

	mainCamera = std::make_shared<Camera>();
	mainCamera->Position = glm::vec3(3.0f, 3.0f, 3.0f);
	mainCamera->LookAt(glm::vec3(0.0f, 0.0f, 0.0f));

	Scene scene{ mainCamera };
	setupScene(scene);
	
	Timer frameTimer{};

	while (!glfwWindowShouldClose(window))
	{
		float deltaTime = frameTimer.Poll();
		frameTimer.Start();

		scene.Update(window, deltaTime);
		scene.Render();
		imguiRender(scene, deltaTime); 

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	cleanupImGui();
	cleanupWindow(window);
	return 0;
}

static void setupScene(Scene& scene)
{
	// TODO : add tents.
	// TODO : add my designed mesh.
	// TODO : add 2 animations (what can these be?)
	// TODO : make crate, barrel and car have specular maps
	// TODO : maybe do normal maps, if have time?
	// TODO : allow to change to day (if have time)
	// TODO : allow to turn the flashlight on and off 
	// TODO : handle required acmera cwitching from drone to walk mode. (remember frone mode weird controls)
	// TODO : add a sky box (have clouds as mesh made from primitives? There are lighting issues with that)
	// TODO : different skybox for night and day?

	setupDirectionalLight(scene);
	setupSpotlights(scene);
	//setupPointLights(scene);

	auto basicLitShader = Shaders::basicLit();
	
	auto whiteUnlit = Shaders::plainUnlit();
	whiteUnlit->use();
	whiteUnlit->setVec3f("u_color", glm::vec3(1,1,1));

	auto whiteLightMaterial = std::make_shared<xMaterial>(glm::vec3(1.0f, 1.0f, 1.0f), whiteUnlit, std::vector<std::shared_ptr<Texture>>{}, std::vector<std::shared_ptr<Texture>>{}, 0.0f);
	auto defaultMat = xMaterial::Default();

	{
		auto groundDiffuseTex = std::make_shared<Texture>();
		groundDiffuseTex->loadRegularTexture("res/textures/Forest-Ground.png", Texture::TextureFormat::RGB);
		auto groundSpecularTex = Texture::Black();
		auto groundMaterial = std::make_shared<xMaterial>(glm::vec3(1.0f, 1.0f, 1.0f), basicLitShader, 
			std::vector<std::shared_ptr<Texture>>{groundDiffuseTex},
			std::vector<std::shared_ptr<Texture>>{groundSpecularTex}, 128.0f);

		Object ground{ std::make_pair(Primitives::Plane(), groundMaterial) };
		ground.Scale = glm::vec3(200.0f, 1.0f, 200.0f);
		ground.TextureScale = glm::vec2(400.0f, 400.0f);

		scene.MoveObject(ground, "ground");
	}

	{
		Object campfire = loadModel("res\\kenney_survival-kit\\modified\\campfire.fbx");
		campfire.Position = glm::vec3(2.53f, 0.0f, -1.09f);
		campfire.Scale = glm::vec3(1.15f);
		scene.MoveObject(campfire, "campfire");

		PointLight fireLight;
		fireLight.position = glm::vec3(2.53f, 0.0f, -1.09f);
		fireLight.color = Consts::fireYellow;
		// for distances see https://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation
		fireLight.constant = 0.6f; // the lower the brighter
		fireLight.linear = 0.22f;
		fireLight.quadratic = 0.20f;
		scene.AddPointLight(fireLight, "fire");

		// TODO : make the color of this fire also change (if have time)
		auto yellowUnlit = Shaders::plainUnlit();
		yellowUnlit->use();
		yellowUnlit->setVec3f("u_color", Consts::fireYellow);
		auto yellowLightMaterial = std::make_shared<xMaterial>(glm::vec3(/*doesn't matter*/), yellowUnlit,
			std::vector<std::shared_ptr<Texture>>{}, std::vector<std::shared_ptr<Texture>>{}, 0.0f);
		Object fire = loadModel("res\\kenney_survival-kit\\modified\\fire.fbx", yellowLightMaterial);
		fire.Position = glm::vec3(2.53f, 0.0f, -1.09f);
		fire.Scale = glm::vec3(1.15f);
		scene.MoveObject(fire, "fire");
	}

	{
		std::vector<std::string> maps
		{
			"res\\textures\\skybox\\right.jpg",
			"res\\textures\\skybox\\left.jpg",
			"res\\textures\\skybox\\top.jpg",
			"res\\textures\\skybox\\bottom.jpg",
			"res\\textures\\skybox\\front.jpg",
			"res\\textures\\skybox\\back.jpg"
		};
		scene.SetupSkybox(maps);
	}

	{
		Object leftHeadlamp { std::make_pair(Primitives::Sphere(), whiteLightMaterial) };
		leftHeadlamp.Position = glm::vec3(2.33f, 0.44f, 1.21f);
		leftHeadlamp.Scale = glm::vec3(0.04, 0.04, 0.04);
		scene.MoveObject(leftHeadlamp, "left headlamp");

		Object rightHeadlamp{ std::make_pair(Primitives::Sphere(), whiteLightMaterial) };
		rightHeadlamp.Position = glm::vec3(2.2f, 0.44f, 1.56f);
		rightHeadlamp.Scale = glm::vec3(0.04, 0.04, 0.04);
		scene.MoveObject(rightHeadlamp, "right headlamp");
	}

	{ 
		// TODO : setup multiple trees (in some manageable way?)
		// TODO : make the tree not shiny
		Object tree = loadModel("res\\kenney_survival-kit\\Models\\FBX format\\tree.fbx");
		tree.Position = glm::vec3(2.0f, 0.0f, -2.0f);
		tree.Scale = glm::vec3(0.13f, 0.13f, 0.13f);
		scene.MoveObject(tree, "tree");
	}

	{
		Object cabin = loadModel("res\\cottage\\cottage_fbx.fbx");
		cabin.Rotation = glm::vec3(-90, 0, 16.5f);
		cabin.Scale = glm::vec3(0.1f, 0.1f, 0.1f);
		scene.MoveObject(cabin, "cabin");
	}

	{
		Object barrel = loadModel("res\\crateandbarrel\\barrel_mesh.obj");
		barrel.Position = glm::vec3(1.13f, 0.0f, -0.93f);
		barrel.Rotation = glm::vec3(0.0f, 71.0f, 0.0f);
		barrel.Scale = glm::vec3(0.43f, 0.59f, 0.43f);
		scene.MoveObject(barrel, "barrel_1");
	}

	{
		Object barrel = loadModel("res\\crateandbarrel\\barrel_mesh.obj");
		barrel.Position = glm::vec3(1.06f, 0.0f, -1.18f);
		barrel.Rotation = glm::vec3(0.0f, 71.0f, 0.0f);
		barrel.Scale = glm::vec3(0.43f, 0.59f, 0.43f); 
		scene.MoveObject(barrel, "barrel_2");
	}

	{
		Object crate = loadModel("res\\crateandbarrel\\crate_mesh.obj");
		crate.Position = glm::vec3(1.59f, 0.0f, -2.14f);
		crate.Rotation = glm::vec3(0.0f, -17.0f, 0.0f);
		crate.Scale = glm::vec3(0.60f, 0.60f, 0.60f);
		scene.MoveObject(crate, "crate");
	}

	{
		Object jeep = loadModel("res\\jeep\\willy's_jeep.fbx");
		jeep.Position = glm::vec3(2.88f, 0.0f, 1.61f);
		jeep.Rotation = glm::vec3(0.0f, -108.5f, 0.0f);
		jeep.Scale = glm::vec3(0.35f, 0.35f, 0.35f);
		scene.MoveObject(jeep, "jeep");
	}

	// TODO : find a different way to handle object composed of 4 primitives.
	// TODO : revisit if have time
	//Object bushFromPrimitives{ std::vector{
	//	//std::make_pair(Primitives::Sphere(glm::vec3(0.0,0.0,0.0), glm::vec3(1.0,1.0,1.0)), defaultMat),
	//	//std::make_pair(Primitives::Sphere(glm::vec3(1.2,0.3,0.0), glm::vec3(0.6,0.6,0.6)), defaultMat),
	//	//std::make_pair(Primitives::Sphere(glm::vec3(0.7,0.1,0.4), glm::vec3(0.6,0.6,0.6)), defaultMat),
	//	
	//	// this is darker for some reason? if further from origin, the mesh appears darker? normals issue?
	//	std::make_pair(Primitives::Sphere(glm::vec3(6.0,0.0,0.0), glm::vec3(1.0,1.0,1.0)), defaultMat), 
	//}};
}

static void setupDirectionalLight(Scene& scene)
{
	DirectionalLight directionalLight;
	directionalLight.color = glm::vec3(0.1f, 0.1f, 0.4f);
	directionalLight.direction = glm::vec3(0.0f, -1.0f, -1.0f);
	scene.SetDirectionalLight(directionalLight);
}

static void setupSpotlights(Scene& scene)
{
	{
		SpotLight flashLight;
		flashLight.color = glm::vec3(1.0f, 1.0f, 1.0f);
		flashLight.constant = 0.4f; // lower = brighter
		flashLight.linear = 0.09f;
		flashLight.quadratic = 0.032f;
		flashLight.cutOff = glm::cos(glm::radians(12.5f));
		flashLight.outerCutOff = glm::cos(glm::radians(15.0f));
		//scene.MoveFlashLight(flashLight);
	}

	auto whiteUnlit = Shaders::plainUnlit();
	whiteUnlit->use();
	whiteUnlit->setVec3f("u_color", glm::vec3(1, 1, 1));
	auto lightMaterial = std::make_shared<xMaterial>(glm::vec3(), whiteUnlit,
		std::vector<std::shared_ptr<Texture>>{}, std::vector<std::shared_ptr<Texture>>{}, 0.0f);

	{
		SpotLight headLight;
		headLight.position = glm::vec3(2.33f, 0.44f, 1.21f);
		glm::mat4 rotation = glm::mat4(1.0f);
		rotation = glm::rotate(rotation, glm::radians(0.0f), glm::vec3(1, 0, 0));
		rotation = glm::rotate(rotation, glm::radians(-25.0f), glm::vec3(0, 1, 0)); // -10 for the other one
		rotation = glm::rotate(rotation, glm::radians(12.0f), glm::vec3(0, 0, 1));
		headLight.direction = -glm::vec3(rotation * glm::vec4(1, 0, 0, 1));
		headLight.color = glm::vec3(1.0f, 1.0f, 1.0f);
		headLight.constant = 0.4f; // lower = brighter
		headLight.linear = 0.09f;
		headLight.quadratic = 0.032f;
		headLight.cutOff = glm::cos(glm::radians(10.0f));
		headLight.outerCutOff = glm::cos(glm::radians(25.0f));
		scene.AddSpotLight(headLight);
	}

	{
		SpotLight headLight;
		headLight.position = glm::vec3(2.2f, 0.44f, 1.56f);
		glm::mat4 rotation = glm::mat4(1.0f);
		rotation = glm::rotate(rotation, glm::radians(0.0f), glm::vec3(1, 0, 0));
		rotation = glm::rotate(rotation, glm::radians(-10.0f), glm::vec3(0, 1, 0));
		rotation = glm::rotate(rotation, glm::radians(12.0f), glm::vec3(0, 0, 1));
		headLight.direction = -glm::vec3(rotation * glm::vec4(1, 0, 0, 1));
		headLight.color = glm::vec3(1.0f, 1.0f, 1.0f);
		headLight.constant = 0.4f; // lower = brighter
		headLight.linear = 0.09f;
		headLight.quadratic = 0.032f;
		headLight.cutOff = glm::cos(glm::radians(10.0f));
		headLight.outerCutOff = glm::cos(glm::radians(25.0f));
		scene.AddSpotLight(headLight);
	}
}

static void setupPointLights(Scene& scene)
{
	{
		PointLight light;
		light.position = glm::vec3(2.0f, 0.25f, 2.0f);
		light.color = glm::vec3(1.0f, 1.0f, 1.0f) * 0.4f;
		// this cover the distance of 50. for other distances see https://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation
		light.constant = 0.2f;
		light.linear = 0.09f;
		light.quadratic = 0.032f;
		//scene.AddPointLight(light, "1");
	}

	{
		PointLight light;
		light.position = glm::vec3(-3.0f, 0.5f, 1.0f);
		light.color = glm::vec3(0.0f, 0.0f, 1.0f) * 0.3f;
		// this cover the distance of 50. for other distances see https://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation
		light.constant = 0.2f;
		light.linear = 0.09f;
		light.quadratic = 0.032f;
		//scene.AddPointLight(light);
	}

	{
		PointLight light;
		light.position = glm::vec3(+1.5f, 0.35f, -1.0f);
		light.color = glm::vec3(0.0f, 1.0f, 0.0f) * 0.3f;
		// this cover the distance of 50. for other distances see https://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation
		light.constant = 0.2f;
		light.linear = 0.09f;
		light.quadratic = 0.032f;
		//scene.AddPointLight(light);
	}
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

static void imguiRender(Scene& scene, float deltaTime) // no const ref cuz ImGui modifies member variables
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("CG Final GUI");
	{
		ImGui::SetWindowFontScale(2.0f);
		auto& io = ImGui::GetIO();
		ImGui::Text("Frame avg: %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate); // rolling average
	
		scene.OnImGuiRender();
	}
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
	glfwSetMouseButtonCallback(*window, mouse_button_callback);
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
	{
		if (mainCamera->IsLocked())
		{
			// TODO : might wanna get rid of this.
			glfwSetWindowShouldClose(window, true);
		}
		else
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			mainCamera->Lock();
		}
	}
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

	mainCamera->ProcessMouseMovement(xoffset, yoffset); // how do I pass camera to this function, when I can't modify its interface?
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	mainCamera->ProcessMouseScroll(static_cast<float>(yoffset));
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (ImGui::GetIO().WantCaptureMouse)
		return;

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && mainCamera->IsLocked())
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		mainCamera->Unlock();
	}
}