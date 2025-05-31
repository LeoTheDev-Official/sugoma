#pragma once
#include "assets/asset.h"
#include "audio_clip.h"

namespace sugoma
{
	using namespace sugoma::audio;
	template<>
	class AssetImpl<AudioClip>
	{
	public:
		static inline constexpr const char* Name() { return "AudioClip"; }
		static inline constexpr AssetType Type() { return (AssetType)AssetTypes::AudioClip; }

		static Buffer PackAssetFile(const std::filesystem::path& path, const AssetMetadata* meta);
		static Buffer PackAsset(const Asset& asset, AssetMetadata* meta);
		static Ref<AudioClip> UnpackAsset(Buffer buffer);
		static AssetMetadata* CreateMetadata();
		static const std::vector<std::string>& Extensions();
	};
}