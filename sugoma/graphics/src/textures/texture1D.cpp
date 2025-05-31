#include "texture1D.h"
#include "resources/resource.h"
#include <GL/glew.h>

namespace sugoma::graphics 
{
	Texture1D::Texture1D(const TextureCreateInfo& createInfo, void* buffer, TextureFormat buffer_format, TextureFormatComponent component) : Texture(createInfo)
	{
		glBindTexture(GL_TEXTURE_1D, m_handle);
		glTexImage1D(GL_TEXTURE_1D, 0, createInfo.format, createInfo.width, createInfo.border, buffer_format, component, buffer);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, createInfo.horizontalWrap);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, createInfo.verticalWrap);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, createInfo.minFilter);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, createInfo.magFilter);
		glBindTexture(GL_TEXTURE_1D, 0);
	}
	Texture1D::~Texture1D()
	{

	}
	Ref<Texture1D> Texture1D::Create(const TextureCreateInfo& createInfo, void* buffer, TextureFormat buffer_format, TextureFormatComponent component)
	{
		return Resources::Create<Texture1D>(createInfo, buffer, buffer_format, component);
	}
	void Texture1D::Reinitialize(uint32_t width, uint32_t height, uint32_t depth, void* buffer, TextureFormat bufferFormat, TextureFormatComponent componentFormat)
	{
		glBindTexture(GL_TEXTURE_1D, m_handle);
		glTexImage1D(GL_TEXTURE_1D, 0, m_info.format, width, m_info.border, bufferFormat, componentFormat, buffer);
		glBindTexture(GL_TEXTURE_1D, 0);
	}
}