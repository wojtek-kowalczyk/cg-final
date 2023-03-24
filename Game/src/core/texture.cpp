#include "texture.h"
#include "glad/glad.h"
#include "stb_image/stb_image.h"
#include "log.h"

Texture::Texture(const std::string& path) : m_rendererId(0), m_width(0), m_height(0), m_bitsPerPixel(0)
{
	glGenTextures(1, &m_rendererId);
	glBindTexture(GL_TEXTURE_2D, m_rendererId);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // nearest is the other option

	stbi_set_flip_vertically_on_load(true); // if textures appear upside down (most textures have 0,0 at the top, opengl wants bottom) dep. on tex format
	unsigned char* data = stbi_load(path.c_str(), &m_width, &m_height, &m_bitsPerPixel, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); // TODO : modes (GL_RGB... stuff)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		Debug::log(std::string("Failed to load texture from path: ") + std::string(path) + '\n');
	}

	stbi_image_free(data);
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_rendererId);
}

void Texture::Bind(unsigned int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_rendererId);
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
