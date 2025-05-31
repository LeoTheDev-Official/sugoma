#include "texture2D.h"
#include <GL/glew.h>
namespace sugoma::graphics 
{
	Texture2D::Texture2D(const TextureCreateInfo& info, void* buffer, TextureFormat buffer_format, TextureFormatComponent component) : Texture(info)
	{
		glBindTexture(GL_TEXTURE_2D, m_handle);
		glTexImage2D(GL_TEXTURE_2D, 0, info.format, info.width, info.height, info.border, buffer_format, component, buffer);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, info.horizontalWrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, info.verticalWrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, info.minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, info.magFilter);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	Texture2D::~Texture2D()
	{
	}
	Ref<Texture2D> Texture2D::Create(const TextureCreateInfo& info, void* buffer, TextureFormat buffer_format, TextureFormatComponent component)
	{
		return Resources::Create<Texture2D>(info, buffer, buffer_format, component);
	}
	void Texture2D::Reinitialize(uint32_t width, uint32_t height, uint32_t depth, void* buffer, TextureFormat bufferFormat, TextureFormatComponent componentFormat)
	{
		glBindTexture(GL_TEXTURE_2D, m_handle);
		glTexImage2D(GL_TEXTURE_2D, 0, m_info.format, width, height, m_info.border, bufferFormat, componentFormat, buffer);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}