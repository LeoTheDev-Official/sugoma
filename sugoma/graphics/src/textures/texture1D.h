#pragma once
#include "texture.h"

namespace sugoma::graphics 
{
	class Texture1D : public Texture 
	{
	public:
		Texture1D() = delete;
		Texture1D(const TextureCreateInfo& createInfo, void* buffer, TextureFormat buffer_format, TextureFormatComponent component);
		~Texture1D();
		static Ref<Texture1D> Create(const TextureCreateInfo& createInfo, void* buffer, TextureFormat buffer_format, TextureFormatComponent component);
		void Reinitialize(uint32_t width, uint32_t height, uint32_t depth, void* buffer, TextureFormat bufferFormat, TextureFormatComponent componentFormat) override;
	};
}