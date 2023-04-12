#pragma once

#include "camera.h"
#include "object.h"
#include "renderer.h"
#include "lights.h"
#include "behaviours/behaviour.h"

#include <GLFW/glfw3.h>

#include <vector>
#include <memory>

class Scene
{
public:
	Scene(const std::shared_ptr<Camera>& camera);

	void Update(GLFWwindow* window, float deltaTime);
	void Render() const;
	void OnImGuiRender() const;
	void MoveObject(Object& object); // transfers ownership to the scene
	void AddPointLight(const PointLight& light);
	void SetDirectionalLight(const DirectionalLight& light);
	void SetSpotLight(const SpotLight& light);

private:
	// TODO multiple camera system.
	std::shared_ptr<Camera> m_camera; // despite camera being integral part of the scene, I need it passed from main() since glfw input callbacks. // TODO : figure this out?
	Renderer m_renderer;
	std::vector<Object> m_objects;
	std::vector<PointLight> m_pointLights;
	DirectionalLight m_directionalLight;
	SpotLight m_spotlight; // currently - flashlight. might wanna consider more.
};

