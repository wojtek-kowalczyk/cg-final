#pragma once

#include <glm/glm.hpp>

namespace Consts
{
	/// use c++17 for inline variables

	// general
	inline constexpr int SCREEN_WIDTH { 1920 };
	inline constexpr int SCREEN_HEIGHT{ 1080 };

	// ui
	inline constexpr float IMGUI_UI_SCALE{ 2.0f };

	// colors
	inline constexpr glm::vec3 fireYellow{ glm::vec3(255.0f / 255.0f, 253.0f / 255.0f, 153.0f / 255.0f) };
}