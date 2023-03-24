#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"

class Scene
{
public:
	virtual void OnUpdate(GLFWwindow* window, float deltaTime) { };
	virtual void OnRender() { };
	virtual void OnImGuiRender() { };
};
