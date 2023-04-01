#include "scene.h"
#include "shader.h"
#include "../consts.h"
#include "texture.h"

Scene::Scene(const std::shared_ptr<Camera>& camera) : m_camera(camera), m_directionalLight(), m_spotlight()
{
	m_renderer.SetClearFlags(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	m_renderer.SetMode(Renderer::Mode::Triangles);
}

void Scene::AddObject(const Object& object)
{
	m_objects.push_back(object);
}

void Scene::AddPointLight(const PointLight& light)
{
	m_pointLights.push_back(light);
}

void Scene::SetDirectionalLight(const DirectionalLight& light)
{
	m_directionalLight = light;
}

void Scene::SetSpotLight(const SpotLight& light)
{
	m_spotlight = light;
}

void Scene::Update(GLFWwindow* window, float deltaTime)
{
	// These are not handled in a callback because of the delay between key press and key repeat

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		m_camera->ProcessKeyboard(Camera::MovementDirection::Forward, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		m_camera->ProcessKeyboard(Camera::MovementDirection::Backward, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		m_camera->ProcessKeyboard(Camera::MovementDirection::Left, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		m_camera->ProcessKeyboard(Camera::MovementDirection::Right, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		m_camera->ProcessKeyboard(Camera::MovementDirection::Down, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		m_camera->ProcessKeyboard(Camera::MovementDirection::Up, deltaTime);

	m_spotlight.position = m_camera->Position;
	m_spotlight.direction = m_camera->Forward;
}

void Scene::Render() const
{
	m_renderer.Clear();

	// If we don't use zoom this doesn't need to change every frame. Only when one of its settings changes.

	glm::mat4 projection = m_camera->GetProjectionMatrix();
	glm::mat4 view = m_camera->GetViewMatrix();

	for (const auto& object : m_objects)
	{
		glm::mat4 model = object.GetModelMatrix();

		Shader& shader = *(object.m_material->MainShader);
		shader.use();
		
		shader.setMat4f("u_v", view);
		shader.setMat4f("u_p", projection);
		shader.setMat4f("u_m", model);

		if (object.m_material->DiffuseMap != nullptr)
			object.m_material->DiffuseMap->Bind(0);
		if (object.m_material->SpecularMap != nullptr)
			object.m_material->SpecularMap->Bind(1);

		shader.setFloat("u_mat.shininess", object.m_material->Shininess);
		shader.setVec3f("u_mat.albedo", object.m_material->Albedo);
		shader.setInt("u_mat.diffuse", object.m_material->DiffuseTextureSlot);
		shader.setInt("u_mat.specular", object.m_material->SpecularTextureSlot);

		shader.setVec3f("u_dirLight.direction", m_directionalLight.direction);
		shader.setVec3f("u_dirLight.color", m_directionalLight.color);

		shader.setInt("u_numLights", m_pointLights.size());
		char buffer[50];
		for (int i = 0; i < m_pointLights.size(); i++)
		{
			sprintf_s(buffer, "u_pointLights[%d].position", i);
			shader.setVec3f(buffer, m_pointLights[i].position);
			sprintf_s(buffer, "u_pointLights[%d].color", i);
			shader.setVec3f(buffer, m_pointLights[i].color);
			sprintf_s(buffer, "u_pointLights[%d].constant", i);
			shader.setFloat(buffer, m_pointLights[i].constant); // lower = brighter
			sprintf_s(buffer, "u_pointLights[%d].liner", i);
			shader.setFloat(buffer, m_pointLights[i].linear);
			sprintf_s(buffer, "u_pointLights[%d].quadratic", i);
			shader.setFloat(buffer, m_pointLights[i].quadratic);
		}

		shader.setVec3f("u_spotLight.position", m_spotlight.position);
		shader.setVec3f("u_spotLight.direction", m_spotlight.direction);

		shader.setVec3f("u_spotLight.color", m_spotlight.color);
		shader.setFloat("u_spotLight.constant", m_spotlight.constant); // lower = brighter
		shader.setFloat("u_spotLight.linear", m_spotlight.linear);
		shader.setFloat("u_spotLight.quadratic", m_spotlight.quadratic);
		shader.setFloat("u_spotLight.cutOff", m_spotlight.cutOff);
		shader.setFloat("u_spotLight.outerCutOff", m_spotlight.outerCutOff);

		shader.setVec3f("u_viewerPos", m_camera->Position);

		for (const auto& mesh : object.m_meshes)
		{
			m_renderer.Draw(*mesh, shader);
		}
	}
}

void Scene::OnImGuiRender() const
{
}
