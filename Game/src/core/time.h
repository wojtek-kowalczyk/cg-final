#pragma once

#include <GLFW/glfw3.h>

// TODO : how are classes with static functions and static state different from namespaces with functions with static local variables?

// TODO : might wanna rethink or devise a more robust deltatime solution.
class Timer
{
public:
	void Start() { m_startTime = static_cast<float>(glfwGetTime()); }
	float Poll() const { return static_cast<float>(glfwGetTime()) - m_startTime; }

private:
	float m_startTime;
};