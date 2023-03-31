#include "scene.h"
#include "shader.h"
#include "../consts.h"
#include "texture.h"

Scene::Scene(const std::weak_ptr<Camera>& camera) : m_camera(camera)
{
	m_renderer.SetClearFlags(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	m_renderer.SetMode(Renderer::Mode::Triangles);
}

void Scene::AddObject(const Object& object)
{
	m_objects.push_back(object);
}

void Scene::Update(GLFWwindow* window, float deltaTime)
{
	// These are not handled in a callback because of the delay between key press and key repeat

	auto cam = m_camera.lock();

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cam->ProcessKeyboard(Camera::MovementDirection::Forward, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cam->ProcessKeyboard(Camera::MovementDirection::Backward, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cam->ProcessKeyboard(Camera::MovementDirection::Left, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cam->ProcessKeyboard(Camera::MovementDirection::Right, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		cam->ProcessKeyboard(Camera::MovementDirection::Down, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		cam->ProcessKeyboard(Camera::MovementDirection::Up, deltaTime);
}

void Scene::Render() const
{
	m_renderer.Clear();

	auto cam = m_camera.lock();

	// If we don't use zoom this doesn't need to change every frame. Only when one of its settings changes.
	glm::mat4 projection = glm::perspective(glm::radians(cam->Zoom), (float)Consts::SCREEN_WIDTH / (float)Consts::SCREEN_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = cam->GetViewMatrix();

	for (const auto& object : m_objects)
	{
		glm::mat4 model = object.GetModelMatrix();
		Shader& shader = *(object.m_material->MainShader);
		shader.use();
		// TODO : isn't setting view and projection for every object a waste if we use only one shader for all objects?
		// or can you access vertex and fragment shader seaprately? so I could have all object share the vertex shader but different fragment shaders?
		shader.setMat4f("u_v", view); 
		shader.setMat4f("u_p", projection);
		shader.setMat4f("u_m", model);
		
		// material properties
		shader.setFloat("u_mat.shininess", 128.0f);

		shader.setVec3f("u_light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		shader.setVec3f("u_light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
		shader.setVec3f("u_light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		shader.setVec3f("u_light.position", glm::vec3(-5, 5, -5));

		shader.setVec3f("u_viewerPos", m_camera.lock()->Position); // TODO : where does the shared ptr we create by calling .lock() go out of scope?

		for (const auto& mesh : object.m_meshes)
		{
			m_renderer.Draw(*mesh, shader);
		}
	}
}

void Scene::OnImGuiRender()
{
}
