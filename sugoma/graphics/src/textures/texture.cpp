#include "texture.h"

#include "debug/logger.h"
#include <GL/glew.h>

namespace sugoma::graphics 
{
	Texture::Texture(const TextureCreateInfo& info) : m_info(info)
	{
		glGenTextures(1, &m_handle);
	}
	Texture::~Texture()
	{
		if (m_handle)
			glDeleteTextures(1, &m_handle);
	}
	GLHandle Texture::Handle() const { return m_handle; }
	void Texture::Reinitialize(uint32_t width, uint32_t height, uint32_t depth) { this->Reinitialize(width, height, depth, nullptr, TextureFormat::RED, TextureFormatComponent::BYTE); }
	uint32_t Texture::Width() const { return m_info.width; }
	uint32_t Texture::Height() const { return m_info.height; }
	uint32_t Texture::Depth() const { return m_info.depth; }
	uint32_t Texture::Layers() const { return m_info.layers; }
	const TextureCreateInfo& Texture::Info() const { return m_info; }
}