#include "texture3D.h"
#include <GL/glew.h>
namespace sugoma::graphics 
{
	Texture3D::Texture3D(const TextureCreateInfo& info, void* buffer, TextureFormat buffer_format, TextureFormatComponent component) : Texture(info)
	{
		glBindTexture(GL_TEXTURE_3D, m_handle);
		glTexImage3D(GL_TEXTURE_3D, 0, info.format, info.width, info.height, info.depth, info.border, buffer_format, component, buffer);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, info.horizontalWrap);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, info.verticalWrap);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, info.minFilter);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, info.magFilter);
		glBindTexture(GL_TEXTURE_3D, 0);
	}
	Texture3D::~Texture3D()
	{

	}
	Ref<Texture3D> Texture3D::Create(const TextureCreateInfo& info, void* buffer, TextureFormat buffer_format, TextureFormatComponent component)
	{
		return Resources::Create<Texture3D>(info, buffer, buffer_format, component);
	}
	void Texture3D::Reinitialize(uint32_t width, uint32_t height, uint32_t depth, void* buffer, TextureFormat bufferFormat, TextureFormatComponent componentFormat)
	{
		glBindTexture(GL_TEXTURE_3D, m_handle);
		glTexImage3D(GL_TEXTURE_3D, 0, m_info.format, width, height, depth, m_info.border, bufferFormat, componentFormat, buffer);
		glBindTexture(GL_TEXTURE_3D, 0);
	}
}