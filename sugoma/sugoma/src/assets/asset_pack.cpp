#include <filesystem>
#include <fstream>
#include "asset_pack.h"
#include "utility/saf_utility.h"
#include "debug/logger.h"
namespace sugoma 
{
	AssetPack::AssetPack(const std::string& path) : m_path(path){}
	void AssetPack::Load()
	{
		std::filesystem::path p = m_path;
		if (std::filesystem::exists(p)) 
		{
			SAFInfo info{};
			SAFUtility::ReadSAF(m_path, info); //FIXME : Taking forever to read for some reason
			std::ifstream stream(m_path, std::ios::binary);
			for (auto& entry : info.entries) 
			{
				if (entry.type == 0) continue;
				stream.seekg(entry.offset);
				Buffer b = Buffer(entry.size);
				stream.read((char*)b.Data(), entry.size);

				Asset asset{};
				asset.fullname = entry.fullName;
				asset.name = entry.name;
				asset.guid = entry.guid;
				asset.type = entry.type;
				
				auto ai = Assets::GetAssetTypeInfo(asset.type);
				asset.resource = ai->unpackAsset(b);
				b.Free();

				Assets::RegisterAsset(asset);
				m_asset_guids[asset.fullname] = asset.guid;
			}
			stream.close();
		}
		else 
		{
			//SAF does not exist. Treat the directory as a sparse SAF
			std::filesystem::path dir_path = p.parent_path() / p.filename().stem();
			if (!std::filesystem::exists(dir_path))
			{
				sugoma_error("Could not load AssetPack. Invalid path : " << dir_path.string());
				return;
			}
			std::filesystem::recursive_directory_iterator it(dir_path);
			for (auto& entry : it) 
			{
				if (entry.is_directory()) continue;
				auto path = entry.path();
				auto ext = path.filename().extension();
				if (ext != ".meta") continue;
				auto fpath = path.parent_path() / path.filename().stem();
				if (!std::filesystem::exists(fpath)) 
				{
					sugoma_error("No matching asset file found for ( " << path << " ).");
					continue;
				}
				ext = fpath.filename().extension();
				AssetType type = Assets::GetTypeFromExtension(ext.string());
				if (!type) 
				{
					sugoma_error("Invalid asset file. ( " << fpath << " )");
					continue;
				}
				auto info = Assets::GetAssetTypeInfo(type);
				AssetMetadata* meta = info->createMetadata();
				std::ifstream stream(path);
				std::string guid_hex;
				stream >> guid_hex;
				meta->Deserialize(stream);
				stream.close();
				Buffer buffer = info->packFile(fpath, meta);
				Ref<Resource> res = info->unpackAsset(buffer);
				buffer.Free();
				
				auto p = std::filesystem::relative(fpath, dir_path);

				Asset asset{};
				asset.guid = guid_hex;
				asset.name = p.filename().stem().string();
				asset.fullname = (p.parent_path() / p.filename().stem()).string();
				asset.resource = res;
				asset.type = type;

				Assets::RegisterAsset(asset);
				m_asset_guids[asset.fullname] = asset.guid;
			}
		}
	}
	void AssetPack::Unload()
	{
		for (auto& guid : m_asset_guids) 
		{
			auto& asset = Assets::GetAssetByGUID(guid.second);
			Resources::Destroy(asset.resource);
			Assets::UnreagisterResource(asset.guid);
		}
		m_asset_guids.clear();
	}
	AssetRef<Resource> AssetPack::FindAsset(const std::string& name)
	{
		auto it = m_asset_guids.find(name);
		if (it == m_asset_guids.end()) return GUID();
		return it->second;
	}
}