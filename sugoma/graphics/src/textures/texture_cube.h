#pragma once
#include "texture.h"
#include "texture2D.h"
namespace sugoma::graphics 
{
	class TextureCube : public Texture 
	{
	public:
		TextureCube() = delete;
		TextureCube(const TextureCreateInfo& info, void** buffers, TextureFormat buffer_format, TextureFormatComponent component);
		~TextureCube();
		static Ref<TextureCube> Create(const TextureCreateInfo& info, void** buffers, TextureFormat buffer_format, TextureFormatComponent component);
		static Ref<TextureCube> FromHDRI(Ref<Texture2D> hdri, const TextureCreateInfo& info);
	};
}