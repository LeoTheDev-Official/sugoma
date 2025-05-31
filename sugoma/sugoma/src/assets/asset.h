#pragma once
#include <iostream>
#include <filesystem>
#include <functional>
#include "resources/resource.h"
#include "memory/buffer.h"
#include "utility/guid.h"
namespace sugoma 
{
	using namespace core;
	using AssetType = int16_t;

	enum class AssetTypes : AssetType
	{
		UNKNOWN = 0,
		Texture = -1,
		Mesh	= -2,
		AudioClip = -3,
	};
	struct Asset 
	{
		GUID guid;
		AssetType type;
		std::string fullname; //the virtual position of the asset in the AssetPack tree
		std::string name;
		Ref<Resource> resource;
	};
	struct AssetMetadata 
	{
		GUID assetGuid;
		virtual void Serialize(std::ostream& stream) const {};
		virtual void Deserialize(std::istream& stream) {};
	};

	template<class T>
	class AssetImpl 
	{
	public:
		static inline constexpr const char* Name() = 0;
		static inline constexpr AssetType Type() = 0;
		static Buffer PackAssetFile(const std::filesystem::path& path, const AssetMetadata* meta) = 0;
		static Buffer PackAsset(const Asset& asset, AssetMetadata* meta) = 0;
		static Ref<Resource> UnpackAsset(Buffer buffer) = 0;
		static const std::vector<std::string>& Extensions() = 0;
		static AssetMetadata* CreateMetadata() = 0;

	};
	using AssetPackFileFunc = std::function<Buffer(const std::filesystem::path&, const AssetMetadata*)>;
	using AssetPackFunc = std::function<Buffer(const Asset&, AssetMetadata*)>;
	using AssetUnpackFunc = std::function<Ref<Resource>(Buffer)>;
	using AssetMetadataFunc = std::function<AssetMetadata*()>;
	using AssetExtensionsFunc = std::function<const std::vector<std::string>&()>;
	struct AssetImplementationInfo 
	{
		AssetType assetType;
		const char* assetTypeName;
		AssetPackFileFunc packFile;
		AssetPackFunc packAsset;
		AssetUnpackFunc unpackAsset;
		AssetMetadataFunc createMetadata;
		AssetExtensionsFunc extensionFunc;
	};
	class Assets 
	{
	public:
		static void RegisterNativeAssetTypes();
		template<class T>
		static void RegisterAssetImplementation()
		{
			using impl = AssetImpl<T>;
			static AssetImplementationInfo info
			{
				.assetType = impl::Type(),
				.assetTypeName = impl::Name(),
				.packFile = impl::PackAssetFile,
				.packAsset = impl::PackAsset,
				.unpackAsset = impl::UnpackAsset,
				.createMetadata = impl::CreateMetadata,
				.extensionFunc = impl::Extensions
			};
			RegisterAssetTypeInfo(info);
		};
		static void RegisterAssetTypeInfo(const AssetImplementationInfo& info);
		static const AssetImplementationInfo* GetAssetTypeInfo(AssetType type);
		static AssetType GetTypeFromExtension(const std::string& ext);

	public:
		static const Asset& GetAssetByGUID(GUID id);
		static Ref<Resource> GetAssetResourceByGUID(GUID id);
		static bool AssetLoaded(GUID id);
		static void RegisterAsset(Asset asset);
		static void UnreagisterResource(GUID guid);
	};
	template<class T>
	class AssetRef
	{
	public:
		AssetRef() = default;
		AssetRef(const std::string& guid) : m_guid(guid) {}
		AssetRef(const GUID& guid) : m_guid(guid) {}
		operator GUID() const { return m_guid; }
		Ref<T> get() { return Assets::GetAssetResourceByGUID(m_guid); }
		const Ref<T> get() const { return Assets::GetAssetResourceByGUID(m_guid); }
		operator bool() const { return Assets::AssetLoaded(m_guid); }
		template<typename B>
		operator AssetRef<B>() { return AssetRef<B>(m_guid); }
		operator Ref<T>() { return Assets::GetAssetResourceByGUID(m_guid); }
	private:
		GUID m_guid{};
	};
}