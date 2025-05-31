#pragma once
#include "asset.h"

namespace sugoma 
{
	class AssetPack 
	{
	public:
		AssetPack() = delete;
		AssetPack(const std::string& path);

		void Load();
		void Unload();
		AssetRef<Resource> FindAsset(const std::string& name);
	private:
		std::string m_path;
		std::unordered_map<std::string, GUID> m_asset_guids;
	};
}