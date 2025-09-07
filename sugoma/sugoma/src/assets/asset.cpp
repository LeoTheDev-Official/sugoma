#include <unordered_map>
#include "asset.h"
#include "debug/logger.h"
#include "assets/types/texture_asset.h"
#include "assets/types/mesh_asset.h"
#include "assets/types/audio_clip_asset.h"

namespace sugoma 
{
    static std::unordered_map<AssetType, AssetImplementationInfo> s_impl;
    static std::unordered_map<std::string, AssetType> s_extensions;

    static std::unordered_map<GUID, Asset> s_loaded_assets;

    void Assets::RegisterNativeAssetTypes()
    {
        RegisterAssetImplementation<Texture>();
        RegisterAssetImplementation<Mesh>();
		RegisterAssetImplementation<AudioClip>();

    }
    void Assets::RegisterAssetTypeInfo(const AssetImplementationInfo& info)
    {
        auto it = s_impl.find(info.assetType);
        if (it != s_impl.end()) return;
        s_impl[info.assetType] = info;
        for (auto& ext : info.extensionFunc())
            s_extensions[ext] = info.assetType;
    }
    const AssetImplementationInfo* Assets::GetAssetTypeInfo(AssetType type)
    {
        auto it = s_impl.find(type);
        if (it == s_impl.end()) return nullptr;
        return &it->second;
    }
    AssetType Assets::GetTypeFromExtension(const std::string& ext)
    {
        auto it = s_extensions.find(ext);
        if (it == s_extensions.end()) return 0;
        return it->second;
    }
    const Asset& Assets::GetAssetByGUID(GUID id)
    {
        auto it = s_loaded_assets.find(id);
        if (it != s_loaded_assets.end()) return it->second;
        return Asset{}; //TODO : fix this
    }
    Ref<Resource> Assets::GetAssetResourceByGUID(GUID id)
    {
        auto it = s_loaded_assets.find(id);
        if (it != s_loaded_assets.end()) return it->second.resource;
        return 0;
    }
    void Assets::RegisterAsset(Asset asset)
    {
        auto it = s_loaded_assets.find(asset.guid);
        if (it != s_loaded_assets.end()) { sugoma_error("Could not register Asset with GUID { " << asset.guid.ToString() << " } as it is already loaded."); return; }
        s_loaded_assets[asset.guid] = asset;
    }
    void Assets::UnreagisterResource(GUID guid)
    {
        auto it = s_loaded_assets.find(guid);
        if (it == s_loaded_assets.end()) { sugoma_error("Could not unregister Asset with GUID { " << guid.ToString() << " } as it is not loaded."); return; }
        s_loaded_assets.erase(guid);
    }
    bool Assets::AssetLoaded(GUID id)
    {
        auto it = s_loaded_assets.find(id);
        if (it != s_loaded_assets.end()) return true;
        return false;
    }
}