#pragma once
#include "texture.h"

namespace sugoma::graphics 
{
	class Texture2D : public Texture 
	{
	public:
		Texture2D() = delete;
		Texture2D(const TextureCreateInfo& info, void* buffer, TextureFormat buffer_format, TextureFormatComponent component);
		~Texture2D();
		static Ref<Texture2D> Create(const TextureCreateInfo& info, void* buffer, TextureFormat buffer_format, TextureFormatComponent component);
		void Reinitialize(uint32_t width, uint32_t height, uint32_t depth, void* buffer, TextureFormat bufferFormat, TextureFormatComponent componentFormat) override;
	};
}