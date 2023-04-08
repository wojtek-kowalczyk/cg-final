#pragma once

#include "glad/glad.h"

#include <string>
#include <memory>

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
	void Unbind(unsigned int slot);
	unsigned int Id() const { return m_rendererId; }

	static std::shared_ptr<Texture> White();
	static std::shared_ptr<Texture> Black();

private:
	GLint GetGLEnumForTextureFormat(Texture::TextureFormat format);

	unsigned int m_rendererId;
	int m_width;
	int m_height;
	int m_bitsPerPixel;
};