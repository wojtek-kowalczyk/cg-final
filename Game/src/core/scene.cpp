#include "scene.h"
#include "shader.h"
#include "../consts.h"
#include "texture.h"

Scene::Scene(const std::shared_ptr<Camera>& camera)
	: m_camera(camera), m_directionalLight(), m_spotlight(), m_ambientLight(0.1f, 0.1f, 0.1f)
{
	m_renderer.SetClearFlags(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	m_renderer.SetMode(Renderer::Mode::Triangles);
}

void Scene::MoveObject(Object& object)
{
	// I have no idea why I cannot pass Object&& and just push_back(object).
	// It will tell me it uses the push_back overload for const Object& instead.
	// I would prefer this to take Object&& so it's evident in main that this function transfers ownership.
	m_objects.push_back(std::move(object));
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

	for (auto& object : m_objects)
	{
		object.Update(deltaTime);
	}
}

void Scene::Render() const
{
	m_renderer.Clear();

	glm::mat4 projection = m_camera->GetProjectionMatrix();
	glm::mat4 view = m_camera->GetViewMatrix();

	for (const auto& object : m_objects)
	{
		glm::mat4 model = object.GetModelMatrix();

		for (const std::pair<std::shared_ptr<Mesh>, std::shared_ptr<xMaterial>>& meshWithMat : object.m_meshesWithMaterialPtrs)
		{
			Shader& shader = *(meshWithMat.second->MainShader);
			shader.use();

			// TODO : maybe skip lights setup if shader is unlit? introduce a way to differentiate here what type of shader that is.
			char buffer[100]; // this will be painful to debug when I overflow this...
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

			shader.setMat4f("u_v", view);
			shader.setMat4f("u_p", projection);
			shader.setMat4f("u_m", model);

			shader.setVec2f("u_UVScale", object.TextureScale);
			
			const int numDiffuseMaps = meshWithMat.second->DiffuseMaps.size();
			const int numSpecularMaps = meshWithMat.second->SpecularMaps.size();

			// TODO : right now there is only support for one diffuse and one specular texture, despite material holding many
			if (meshWithMat.second->DiffuseMaps.size() > 0)
			{
				meshWithMat.second->DiffuseMaps[0]->Bind(0);
				shader.setInt("u_mat.diffuse", 0);
			}

			if (meshWithMat.second->SpecularMaps.size() > 0)
			{
				meshWithMat.second->SpecularMaps[0]->Bind(1);
				shader.setInt("u_mat.specular", 1);
			}

			shader.setFloat("u_mat.shininess", meshWithMat.second->Shininess);
			shader.setVec3f("u_mat.albedo", meshWithMat.second->Albedo);

			shader.setVec3f("u_dirLight.direction", m_directionalLight.direction);
			shader.setVec3f("u_dirLight.color", m_directionalLight.color);

			shader.setVec3f("u_ambientLight", m_ambientLight);

			shader.setInt("u_numLights", m_pointLights.size());

			m_renderer.Draw(*meshWithMat.first, shader);

			// If they were bound - unbind them - so as not to reuse textures on objects that don't have textures.
			// If an object doesn't have a texture it will be black
			
			if (meshWithMat.second->DiffuseMaps.size() > 0)
			{
				meshWithMat.second->DiffuseMaps[0]->Unbind(0);
			}

			if (meshWithMat.second->SpecularMaps.size() > 0)
			{
				meshWithMat.second->SpecularMaps[0]->Unbind(1);
			}
		}
	}
}

void Scene::OnImGuiRender() const
{
}
