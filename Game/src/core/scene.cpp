#include "scene.h"
#include "shader.h"
#include "../consts.h"
#include "texture.h"
#include "shaders.h"
#include "primitives.h"

#include <imgui/imgui.h>

#include <simplexNoise/SimplexNoise.h>

#include <glm/gtc/type_ptr.hpp>
#include "scene.h"

Scene::Scene(const std::shared_ptr<Camera>& camera)
	: m_camera(camera), m_directionalLight(), m_flashLight(), m_ambientLight(0.1f, 0.1f, 0.1f)
{
	m_renderer.SetClearFlags(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	m_renderer.SetMode(Renderer::Mode::Triangles);
}

void Scene::MoveObject(Object& object, const std::string& id)
{
	// I have no idea why I cannot pass Object&& and just push_back(object).
	// It will tell me it uses the push_back overload for const Object& instead.
	// I would prefer this to take Object&& so it's evident in main that this function transfers ownership.
	m_objects.insert(std::make_pair(id, std::move(object)));
}

void Scene::AddPointLight(const PointLight& light, const std::string& id)
{
	m_pointLights.push_back(light);
	m_pointLightsLookupTable.insert(std::make_pair(id, m_pointLights.size() - 1)); // index of this point light for retireval
}

void Scene::SetDirectionalLight(const DirectionalLight& light)
{
	m_directionalLight = light;
}

void Scene::AddFlashLight(const SpotLight& light)
{
	m_flashLight = light;
}

void Scene::AddSpotLight(const SpotLight& spotlight)
{
	m_spotLights.push_back(spotlight);
}

void Scene::SetupSkybox(const std::vector<std::string>& maps)
{
	auto skyboxDiffuseTex = std::make_shared<Texture>();
	skyboxDiffuseTex->loadCubeMap(maps, Texture::TextureFormat::RGB);

	auto skyboxShader = Shaders::skybox();
	auto skyboxMaterial = std::make_shared<xMaterial>(glm::vec3(1, 1, 1), skyboxShader,
		std::vector<std::shared_ptr<Texture>>{ skyboxDiffuseTex }, std::vector<std::shared_ptr<Texture>>{}, 0.0f);
	m_skybox = std::make_unique<Object>(std::make_pair(Primitives::Cube(), skyboxMaterial));
}

void Scene::renderSkybox() const
{
	glDepthFunc(GL_LEQUAL);

	auto& meshWithMat = m_skybox->m_meshesWithMaterialPtrs[0];
	Shader& shader = *(meshWithMat.second->MainShader);
	shader.use();
	shader.setMat4f("u_v", glm::mat4(glm::mat3(m_camera->GetViewMatrix())));
	shader.setMat4f("u_p", m_camera->GetProjectionMatrix());
	//shader.setMat4f("u_m", model);

	const int numDiffuseMaps = meshWithMat.second->DiffuseMaps.size();

	// TODO : right now there is only support for one diffuse and one specular texture, despite material holding many
	if (meshWithMat.second->DiffuseMaps.size() > 0)
	{
		meshWithMat.second->DiffuseMaps[0]->Bind(0);
		shader.setInt("u_skybox", 0); // skybox tex is bound to slot 0.
	}

	m_renderer.Draw(*meshWithMat.first, shader);
}

static float lerp(float a, float b, float t)
{
	return a + t * (b - a);
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

	m_flashLight.position = m_camera->Position;
	m_flashLight.direction = m_camera->Forward;

	for (auto& object : m_objects)
	{
		object.second.Update(deltaTime);
	}

	// TODO : this code is shoudn't be here but the deadline is close and I don't have time to refactor.
	static float timer = 0.0f;
	static const float speed = 7.0f;
	timer += deltaTime;
	int fireIndex = m_pointLightsLookupTable["fire"];
	float t = (SimplexNoise::noise(timer * speed) + 1.0f) / 2.0f;
	m_pointLights[fireIndex].constant = lerp(0.2f, 0.5f, t);
}

void Scene::Render() const
{
	m_renderer.Clear();
	
	glm::mat4 projection = m_camera->GetProjectionMatrix();
	glm::mat4 view = m_camera->GetViewMatrix();

	for (const auto& object : m_objects)
	{
		glm::mat4 model = object.second.GetModelMatrix();

		for (const std::pair<std::shared_ptr<Mesh>, std::shared_ptr<xMaterial>>& meshWithMat : object.second.m_meshesWithMaterialPtrs)
		{
			Shader& shader = *(meshWithMat.second->MainShader);
			shader.use();

			// TODO : maybe skip lights setup if shader is unlit? introduce a way to differentiate here what type of shader that is.

			shader.setInt("u_numPointLights", m_pointLights.size());
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

			shader.setInt("u_numSpotLights", m_spotLights.size());
			for (int i = 0; i < m_spotLights.size(); i++)
			{
				const SpotLight& spotLight = m_spotLights[i];

				sprintf_s(buffer, "u_spotLights[%d].position", i);
				shader.setVec3f(buffer, spotLight.position);

				sprintf_s(buffer, "u_spotLights[%d].direction", i);
				shader.setVec3f(buffer, spotLight.direction);

				sprintf_s(buffer, "u_spotLights[%d].color", i);
				shader.setVec3f(buffer, spotLight.color);

				sprintf_s(buffer, "u_spotLights[%d].constant", i);
				shader.setFloat(buffer, spotLight.constant); // lower = brighter

				sprintf_s(buffer, "u_spotLights[%d].linear", i);
				shader.setFloat(buffer, spotLight.linear);

				sprintf_s(buffer, "u_spotLights[%d].quadratic", i);
				shader.setFloat(buffer, spotLight.quadratic);

				sprintf_s(buffer, "u_spotLights[%d].cutOff", i);
				shader.setFloat(buffer, spotLight.cutOff);

				sprintf_s(buffer, "u_spotLights[%d].outerCutOff", i);
				shader.setFloat(buffer, spotLight.outerCutOff);
			}

			shader.setVec3f("u_viewerPos", m_camera->Position);

			shader.setMat4f("u_v", view);
			shader.setMat4f("u_p", projection);
			shader.setMat4f("u_m", model);

			shader.setVec2f("u_UVScale", object.second.TextureScale);
			
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

	renderSkybox();
}

static bool isUniforScale(glm::vec3 scale)
{
	return (scale.x - scale.y) < 0.001f && (scale.y - scale.z) < 0.001f;
}

void Scene::OnImGuiRender()
{
	ImGui::Text("scene objects:");
	for (auto& pair : m_objects)
	{
		if (ImGui::Button(pair.first.c_str()))
		{
			if (m_selectedObject == &pair.second)
			{
				m_selectedObjectId = nullptr;
				m_selectedObject = nullptr;
			}
			else
			{
				m_selectedObjectId = pair.first.c_str();
				m_selectedObject = &pair.second;
				m_scaleUniformly = false; // reset this so that we don't recale the next clicked object
				break;
			}
		}
	}

	if (m_selectedObject != nullptr)
	{
		ImGui::Separator();
		ImGui::Text(m_selectedObjectId);
		ImGui::Separator();

		ImGui::DragFloat3("position", glm::value_ptr(m_selectedObject->Position), 0.01f);
		ImGui::DragFloat3("rotation", glm::value_ptr(m_selectedObject->Rotation), 0.5f);
		ImGui::Checkbox("uniform", &m_scaleUniformly);
		ImGui::SameLine();
		if (!m_scaleUniformly)
		{
			ImGui::DragFloat3("scale", glm::value_ptr(m_selectedObject->Scale), 0.01f);
		}
		else
		{
			ImGui::DragFloat("scale", glm::value_ptr(m_selectedObject->Scale), 0.01f);
			m_selectedObject->Scale.y = m_selectedObject->Scale.x;
			m_selectedObject->Scale.z = m_selectedObject->Scale.x;
		}

		ImGui::Separator();
	}
}
