#pragma once
#include "modules/debug_module.h"
#include "assets/asset.h"
#include <filesystem> 
namespace sugoma
{
	struct APFileEntry 
	{
		std::filesystem::path path;
		std::string name;
		std::string extension;
		std::string hexGUID;
		bool is_directory;
		bool included = false;
		bool opened = false;
		AssetType assetType;
		const char* assetTypeName;
		AssetMetadata* metadata = nullptr;
		std::vector<APFileEntry> children;
	};

	class AssetPacker : public DebugWindow 
	{
	public:
		AssetPacker();
		~AssetPacker();
		void OnCreate() override;
		void OnClose() override;
		void OnGUI() override;

	protected:
		void ClearEntries();
		void RegenEntries();
		void OpenPath(const std::filesystem::path& path);
		APFileEntry LoadEntry(const std::filesystem::path& path);

		void DrawDirectoryEntry(APFileEntry& entry);
		void DrawEntry(APFileEntry& entry);

		void SetSelectedEntry(APFileEntry& entry);

		void DrawEntryHierarchy();
		void DrawEntryMetadata();
		void DrawEntryPreview();
		void SaveSelectedMeta();
		void ReloadPreview();

		void ExportSAF(const std::filesystem::path& destination);

	private:
		APFileEntry m_active;
		std::filesystem::path m_active_path;
		APFileEntry* m_selected = nullptr;
		Ref<Resource> m_preview;
	};
}