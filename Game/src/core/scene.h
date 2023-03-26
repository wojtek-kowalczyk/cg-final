#pragma once

#include "camera.h"
#include "object.h"
#include "renderer.h"

#include <GLFW/glfw3.h>

#include <vector>
#include <memory>
class Scene
{
public:
	Scene(const std::weak_ptr<Camera>& camera);
	void AddObject(const Object& object);
	void Update(GLFWwindow* window, float deltaTime);
	void Render() const;
	void OnImGuiRender();

private:
	// TODO multiple camera system.
	std::vector<Object> m_objects;
	std::weak_ptr<Camera> m_camera;
	Renderer m_renderer;
};
