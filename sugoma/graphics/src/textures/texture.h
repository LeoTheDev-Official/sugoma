#pragma once
#include "resources/resource.h"
#include "sugoma_gl.h"
namespace sugoma::graphics
{
	using namespace sugoma::core;
	enum class TextureDimension
	{
		Texture1D,
		Texture2D,
		Texture3D,
		TextureCube
	};
	struct TextureCreateInfo 
	{
		uint32_t width, height, depth = 1, layers = 1, border = 0;
		TextureFormat format;
		TextureWrapMode verticalWrap = REPEAT, horizontalWrap = REPEAT;
		TextureFilter minFilter = LINEAR, magFilter = LINEAR;
	};
	class Texture : public Resource 
	{
	public:
		virtual ~Texture();
		GLHandle Handle() const;
		virtual void Reinitialize(uint32_t width, uint32_t height, uint32_t depth, void* buffer, TextureFormat bufferFormat, TextureFormatComponent componentFormat) {};
		void Reinitialize(uint32_t width, uint32_t height, uint32_t depth);

		uint32_t Width() const;
		uint32_t Height() const;
		uint32_t Depth() const;
		uint32_t Layers() const;

		const TextureCreateInfo& Info() const;
	protected:
		Texture(const TextureCreateInfo& info);

	protected:
		TextureCreateInfo m_info;
		GLHandle m_handle;
	};
}