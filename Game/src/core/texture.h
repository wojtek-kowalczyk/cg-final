#pragma once

#include <string>

class Texture
{
private:
	unsigned int m_rendererId;
	int m_width;
	int m_height;
	int m_bitsPerPixel;
	std::string filepath;

public:
	Texture(const std::string& path);
	~Texture();

	void Bind(unsigned int slot = 0);
	void Unbind();
	unsigned int Id() { return m_rendererId; }
};