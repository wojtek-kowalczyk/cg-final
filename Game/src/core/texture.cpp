#include "texture.h"
#include "stb_image/stb_image.h"
#include "log.h"

Texture::Texture()
	: m_rendererId(0), m_width(0), m_height(0), m_bitsPerPixel(0), m_isInitialized(false),
		m_type(Texture::TextureType::Invalid)
{
	glGenTextures(1, &m_rendererId);
}

void Texture::loadRegularTexture(const std::string& path, Texture::TextureFormat format)
{
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
		GLint glFormat = GetGLEnumForTextureFormat(format);
		glTexImage2D(GL_TEXTURE_2D, 0, glFormat, m_width, m_height, 0, glFormat, GL_UNSIGNED_BYTE, data); // TODO : modes (GL_RGB... stuff)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		Debug::log(std::string("Failed to load texture from path: ") + std::string(path) + '\n');
	}

	stbi_image_free(data);
	m_isInitialized = true;
	m_type = Texture::TextureType::Regular;
}

void Texture::loadCubeMap(const std::vector<std::string>& paths, Texture::TextureFormat format)
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_rendererId);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	
	unsigned char* data;
	for (unsigned int i = 0; i < paths.size(); i++)
	{
		stbi_set_flip_vertically_on_load(false); // if textures appear upside down (most textures have 0,0 at the top, opengl wants bottom) dep. on tex format
		data = stbi_load(paths[i].c_str(), &m_width, &m_height, &m_bitsPerPixel, 0);
		if (data)
		{
			GLint glFormat = GetGLEnumForTextureFormat(format);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, glFormat, m_width, m_height, 0, glFormat, GL_UNSIGNED_BYTE, data);
			// TODO : mipmaps?
		}
		else
		{
			Debug::log("Failed to load texture from path: " + paths[i] + '\n');
		}
		stbi_image_free(data);
	}

	m_type = Texture::TextureType::Cubemap;
	m_isInitialized = true;
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_rendererId);
}

void Texture::Bind(unsigned int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	switch (m_type)
	{
		case Texture::TextureType::Invalid:
			Debug::log("cannot bind invalid texture!", Debug::ERROR);
			break;
		case Texture::TextureType::Regular:
			glBindTexture(GL_TEXTURE_2D, m_rendererId);
			break;
		case Texture::TextureType::Cubemap:
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_rendererId);
			break;
		default:
			Debug::log("Unhandled Texture type!", Debug::ERROR);
			break;
	}
}

void Texture::Unbind(unsigned int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, 0);
}

std::shared_ptr<Texture> Texture::White()
{
	static auto whiteTex = std::make_shared<Texture>();
	if (whiteTex->m_isInitialized == false)
		whiteTex->loadRegularTexture("res\\textures\\defaultWhite.png", TextureFormat::RGBA);

	return whiteTex;
}

std::shared_ptr<Texture> Texture::Black()
{
	static auto blackTex = std::make_shared<Texture>();
	if (blackTex->m_isInitialized == false)
		blackTex->loadRegularTexture("res\\textures\\defaultBlack.png", TextureFormat::RGBA);

	return blackTex;
}

GLint Texture::GetGLEnumForTextureFormat(Texture::TextureFormat format)
{
	switch (format)
	{
		case Texture::TextureFormat::RGB: return GL_RGB;
		case Texture::TextureFormat::RGBA: return GL_RGBA;
		default: return -1;
	}
}
