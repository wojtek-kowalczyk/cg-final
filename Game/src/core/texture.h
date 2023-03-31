#pragma once

#include "glad/glad.h"

#include <string>

class Texture
{
public:
	enum class TextureFormat
	{
		RGB,
		RGBA,
	};

	Texture(const std::string& path, Texture::TextureFormat format);
	~Texture();

	void Bind(unsigned int slot);
	void Unbind();
	unsigned int Id() const { return m_rendererId; }

private:
	GLint GetGLEnumForTextureFormat(Texture::TextureFormat format);

	unsigned int m_rendererId;
	int m_width;
	int m_height;
	int m_bitsPerPixel;
	std::string filepath;

};