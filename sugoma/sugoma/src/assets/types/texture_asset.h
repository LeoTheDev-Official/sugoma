#pragma once
#include "assets/asset.h"
#include "textures/texture.h"
namespace sugoma 
{
	using namespace sugoma::graphics;

	struct TextureMetadata : public AssetMetadata
	{
		TextureDimension dimension = TextureDimension::Texture2D;
		TextureFilter magFilter = LINEAR, minFilter = LINEAR;
		TextureWrapMode verticalWrap = CLAMP_TO_EDGE, horizontalWrap = CLAMP_TO_EDGE;
		TextureFormat textureFormat = RGBA8;
		bool hdr = false, flip = false;

		void Serialize(std::ostream& stream) const override;
		void Deserialize(std::istream& stream) override;
	};

	template<>
	class AssetImpl<Texture> 
	{
	public:
		static inline constexpr const char* Name() { return "Texture"; }
		static inline constexpr AssetType Type() { return (AssetType)AssetTypes::Texture; };
		static Buffer PackAssetFile(const std::filesystem::path& path, const AssetMetadata* meta);
		static Buffer PackAsset(const Asset& asset, AssetMetadata* meta);
		static Ref<Texture> UnpackAsset(Buffer buffer);
		static AssetMetadata* CreateMetadata();
		static const std::vector<std::string>& Extensions();
	};
}