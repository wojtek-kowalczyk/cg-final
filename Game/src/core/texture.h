#pragma once

#include "glad/glad.h"

#include <string>
#include <memory>
#include <vector>

class Texture
{
public:
	enum class TextureFormat
	{
		RGB,
		RGBA,
	};
	enum class TextureType
	{
		Invalid,
		Regular, 
		Cubemap,
	};

	Texture();
	~Texture();

	void loadRegularTexture(const std::string& path, Texture::TextureFormat format);
	void loadCubeMap(const std::vector<std::string>& paths, Texture::TextureFormat format);

	void Bind(unsigned int slot);
	void Unbind(unsigned int slot);
	unsigned int Id() const { return m_rendererId; }

	static std::shared_ptr<Texture> White();
	static std::shared_ptr<Texture> Black();

private:
	GLint GetGLEnumForTextureFormat(Texture::TextureFormat format);

	TextureType m_type;
	bool m_isInitialized;
	unsigned int m_rendererId;
	int m_width;
	int m_height;
	int m_bitsPerPixel;
};