#pragma once

#include <glm/glm.hpp>

namespace Consts
{
	/// use c++17 for inline variables

	inline constexpr int SCREEN_WIDTH { 1920 };
	inline constexpr int SCREEN_HEIGHT{ 1080 };

	inline constexpr glm::vec3 DEFAULT_CAMERA_POS = glm::vec3(0, 0, 0);

	inline constexpr float IMGUI_UI_SCALE{ 2.0f };

	inline constexpr glm::vec3 FIRE_YELLOW{ glm::vec3(255.0f / 255.0f, 253.0f / 255.0f, 153.0f / 255.0f) };
}