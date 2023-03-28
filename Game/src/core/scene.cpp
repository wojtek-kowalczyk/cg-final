#include "scene.h"
#include "shader.h"
#include <consts.h>


Scene::Scene(const std::weak_ptr<Camera>& camera) : m_camera(camera)
{
	m_renderer.SetClearFlags(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
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

	// TODO : move shader to be a part of material
	// Shader setup can be done outside. mind what things need to update (eg. view matrix coming from "camera")
	Shader shader{ "res/vert.glsl", "res/frag.glsl" };
	auto cam = m_camera.lock();
	// If we don't use zoom this doesn't need to change every frame. Only when one of its settings changes.
	glm::mat4 projection = glm::perspective(glm::radians(cam->Zoom), (float)Consts::SCREEN_WIDTH / (float)Consts::SCREEN_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = cam->GetViewMatrix();

	shader.use();
	shader.setMat4f("u_v", view);
	shader.setMat4f("u_p", projection);

	for (const auto& object : m_objects)
	{
		glm::mat4 model = object.GetModelMatrix();

		for (const auto& mesh : object.m_meshes)
		{
			shader.setMat4f("u_m", model);
			m_renderer.Draw(*mesh, shader);
		}
	}
}

void Scene::OnImGuiRender()
{
}
