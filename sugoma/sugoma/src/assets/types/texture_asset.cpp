#include <fstream>
#include "texture_asset.h"

#include "textures/texture2D.h"
#include "textures/texture_cube.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
namespace sugoma 
{
	static TextureFormat comp_formats[] = { RED, RED, RG, RGB, RGBA };
	struct texture_metadata
	{
		TextureDimension dimension = TextureDimension::Texture2D;
		uint32_t width, height;
		TextureFilter magFilter = LINEAR, minFilter = LINEAR;
		TextureWrapMode verticalWrap = CLAMP_TO_EDGE, horizontalWrap = CLAMP_TO_EDGE;
		TextureFormat textureFormat = RGBA8;
		TextureFormatComponent component = UNSIGNED_BYTE;
	};

	int channel_count_from_format(TextureFormat format) 
	{
		switch (format)
		{
		case RED:
			return 1;
		case RG:
			return 2;
		case RGB:
			return 3;
		case BGR:
			return 3;
		case RGBA:
			return 4;
		case BGRA:
			return 4;
		case DEPTH_COMPONENT:
			return 1;
		case DEPTH_STENCIL:
			return 2;
		case STENCIL_INDEX:
			return 1;
		case LUMINANCE:
			return 1;
		case LUMINANCE_ALPHA:
			return 2;
		case R8:
			return 1;
		case R16:
			return 1;
		case RG8:
			return 2;
		case RG16:
			return 2;
		case R16F:
			return 1;
		case R32F:
			return 1;
		case RG16F:
			return 2;
		case RG32F:
			return 2;
		case R8I:
			return 1;
		case R8UI:
			return 1;
		case R16I:
			return 1;
		case R16UI:
			return 1;
		case R32I:
			return 1;
		case R32UI:
			return 1;
		case RG8I:
			return 2;
		case RG8UI:
			return 2;
		case RG16I:
			return 2;
		case RG16UI:
			return 2;
		case RG32I:
			return 2;
		case RG32UI:
			return 2;
		case RGB8:
			return 3;
		case RGB16:
			return 3;
		case RGB16F:
			return 3;
		case RGB32F:
			return 3;
		case RGB8I:
			return 3;
		case RGB8UI:
			return 3;
		case RGB16I:
			return 3;
		case RGB16UI:
			return 3;
		case RGB32I:
			return 3;
		case RGB32UI:
			return 3;
		case RGBA8:
			return 4;
		case RGBA16:
			return 4;
		case RGBA16F:
			return 4;
		case RGBA32F:
			return 4;
		case RGBA8I:
			return 4;
		case RGBA8UI:
			return 4;
		case RGBA16I:
			return 4;
		case RGBA16UI:
			return 4;
		case RGBA32I:
			return 4;
		case RGBA32UI:
			return 4;
		case SRGB:
			return 3;
		case SRGB8:
			return 3;
		case SRGBA:
			return 4;
		case SRGBA8:
			return 4;
		}
		return 0;
	}

	Buffer AssetImpl<Texture>::PackAssetFile(const std::filesystem::path& path, const AssetMetadata* meta)
	{
		if (!std::filesystem::exists(path)) return {};
		TextureMetadata tmeta{};
		if (meta)
			tmeta = *reinterpret_cast<const TextureMetadata*>(meta);
		int cc = channel_count_from_format(tmeta.textureFormat);
		int width, height, bpp;
		stbi_set_flip_vertically_on_load(!tmeta.flip);
		uint8_t* buffer = nullptr;
		TextureFormatComponent comp;
		uint8_t s = 0;
		if (tmeta.hdr)
		{
			s = sizeof(float);
			comp = TextureFormatComponent::FLOAT;
			buffer = (uint8_t*)stbi_loadf(path.string().c_str(), &width, &height, &bpp, cc);
		}
		else 
		{
			s = sizeof(char);
			comp = TextureFormatComponent::UNSIGNED_BYTE;
			buffer = (uint8_t*)stbi_load(path.string().c_str(), &width, &height, &bpp, cc);
		}

		texture_metadata t{};
		t.dimension = tmeta.dimension;
		t.width = width;
		t.height = height;
		t.magFilter = tmeta.magFilter;
		t.minFilter = tmeta.minFilter;
		t.verticalWrap = tmeta.verticalWrap;
		t.horizontalWrap = tmeta.horizontalWrap;
		t.textureFormat = tmeta.textureFormat;
		t.component = comp;

		size_t img_size = width * height * s * cc;

		Buffer b = Buffer(sizeof(t) + img_size);
		memcpy(b.Data(), &t, sizeof(t));
		memcpy(b.Data() + sizeof(t), buffer, img_size);

		stbi_image_free(buffer);
		return b;
	}
	Buffer AssetImpl<Texture>::PackAsset(const Asset& asset, AssetMetadata* meta)
	{
		return Buffer();
	}
	Ref<Texture> AssetImpl<Texture>::UnpackAsset(Buffer buffer)
	{
		if (!buffer.Data()) return 0;
		texture_metadata t{};
		memcpy(&t, buffer.Data(), sizeof(t));
		void* buff = buffer.Data() + sizeof(t);

		TextureCreateInfo info{};
		info.width = t.width;
		info.height = t.height;
		info.magFilter = t.magFilter;
		info.minFilter = t.minFilter;
		info.horizontalWrap = t.horizontalWrap;
		info.verticalWrap = t.verticalWrap;
		info.format = t.textureFormat;
		
		int cc = channel_count_from_format(t.textureFormat);

		Ref<Texture> result;
		switch (t.dimension)
		{
		case TextureDimension::Texture1D:
			break;
		case TextureDimension::Texture2D:
			result = Texture2D::Create(info, buff, comp_formats[cc], t.component);
			break;
		case TextureDimension::Texture3D:
			break;
		case TextureDimension::TextureCube:
			Ref<Texture2D> staging_texture = Texture2D::Create(info, (void**)buff, comp_formats[cc], t.component);
			info.width = staging_texture->Height() / 2;
			info.height = info.width;
			result = TextureCube::FromHDRI(staging_texture, info);
			Resources::Destroy(staging_texture);
			break;
		}
		return result;
	}
	AssetMetadata* AssetImpl<Texture>::CreateMetadata() { return new TextureMetadata(); }

	const std::vector<std::string>& AssetImpl<Texture>::Extensions()
	{
		static std::vector<std::string> extensions = { ".png", ".jpg", ".jpeg", ".hdr"};
		return extensions;
	}

	void TextureMetadata::Serialize(std::ostream& stream) const
	{
		stream <<
			(uint32_t)dimension << ' ' <<
			(uint32_t)magFilter << ' ' <<
			(uint32_t)minFilter << ' ' <<
			(uint32_t)verticalWrap << ' ' <<
			(uint32_t)horizontalWrap << ' ' <<
			(uint32_t)textureFormat << ' ' <<
			hdr << ' ' << flip;
	}
	void TextureMetadata::Deserialize(std::istream& stream)
	{
		if (stream.eof())
			return;
		uint32_t dim, maf, mif, vw, hw, tf;
		stream >> dim >> maf >> mif >> vw >> hw >> tf >> hdr >> flip;
		dimension = (TextureDimension)dim;
		magFilter = (TextureFilter)maf;
		minFilter = (TextureFilter)mif;
		verticalWrap = (TextureWrapMode)vw;
		horizontalWrap = (TextureWrapMode)hw;
		textureFormat = (TextureFormat)tf;
	}
}