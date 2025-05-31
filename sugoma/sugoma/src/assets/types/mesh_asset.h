#pragma once

#include "assets/asset.h"
#include"mesh/mesh.h"
namespace sugoma 
{
	using namespace sugoma::graphics;
	struct MeshMetadata : public AssetMetadata
	{
		
	};
	template<>
	class AssetImpl<Mesh>
	{
	public:
		static inline constexpr const char* Name() { return "Mesh"; }
		static inline constexpr AssetType Type() { return (AssetType)AssetTypes::Mesh; };
		static Buffer PackAssetFile(const std::filesystem::path& path, const AssetMetadata* meta);
		static Buffer PackAsset(const Asset& asset, AssetMetadata* meta);
		static Ref<Mesh> UnpackAsset(Buffer buffer);
		static AssetMetadata* CreateMetadata();
		static const std::vector<std::string>& Extensions();
	};
}