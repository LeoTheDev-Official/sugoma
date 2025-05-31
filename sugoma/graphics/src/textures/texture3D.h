#pragma once
#include "texture.h"
namespace sugoma::graphics
{
	class Texture3D : public Texture 
	{
	public:
		Texture3D(const TextureCreateInfo& info, void* buffer, TextureFormat buffer_format, TextureFormatComponent component);
		~Texture3D();
		static Ref<Texture3D> Create(const TextureCreateInfo& info, void* buffer, TextureFormat buffer_format, TextureFormatComponent component);
		void Reinitialize(uint32_t width, uint32_t height, uint32_t depth, void* buffer, TextureFormat bufferFormat, TextureFormatComponent componentFormat) override;
	};
}