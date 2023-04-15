#pragma once

#include "camera.h"
#include "object.h"
#include "renderer.h"
#include "lights.h"
#include "behaviours/behaviour.h"

#include <GLFW/glfw3.h>

#include <vector>
#include <string>
#include <memory>
#include <unordered_map>

class Scene
{
public:
	Scene(const std::shared_ptr<Camera>& camera);

	void Update(GLFWwindow* window, float deltaTime);
	void Render() const;
	void OnImGuiRender();
	void MoveObject(Object& object, const std::string& id); // transfers ownership to the scene
	void AddSpotLight(const SpotLight& spotlight);
	void AddFlashLight(const SpotLight& light);
	void AddPointLight(const PointLight& light, const std::string& id);
	void SetDirectionalLight(const DirectionalLight& light);
	void SetupSkybox(const std::vector<std::string>& maps);

private:
	void renderSkybox() const;

	// TODO multiple camera system.
	std::shared_ptr<Camera> m_camera; // despite camera being integral part of the scene, I need it passed from main() since glfw input callbacks. // TODO : figure this out?
	Renderer m_renderer;
	std::unordered_map<std::string, Object> m_objects;
	std::vector<SpotLight> m_spotLights;
	std::vector<PointLight> m_pointLights;
	DirectionalLight m_directionalLight;
	SpotLight m_flashLight; // currently - flashlight. might wanna consider more.
	glm::vec3 m_ambientLight;
	std::unique_ptr<Object> m_skybox;
	std::unordered_map<std::string, unsigned int> m_pointLightsLookupTable;

	// Editor settings
	bool m_scaleUniformly = false;
	Object* m_selectedObject = nullptr;
	const char* m_selectedObjectId = nullptr;
};

