#include "shaders.h"

std::shared_ptr<Shader> Shaders::basicLit()
{
	static auto basicLit = std::make_shared<Shader>("res/shaders/basic.vert", "res/shaders/basicLit.frag");
	return basicLit;
}

std::shared_ptr<Shader> Shaders::plainUnlit()
{
	static auto plainUnlit = std::make_shared<Shader>("res/shaders/basic.vert", "res/shaders/colorOnly.frag");
	return plainUnlit;
}
