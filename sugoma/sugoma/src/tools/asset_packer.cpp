#include <fstream>
#include "asset_packer.h"
#include "imgui.h"
#include "utility/saf_utility.h"
#include "utility/file_utility.h"
#include "../../vendor/graphics/inc/GLFW/glfw3.h"
#include "assets/types/texture_asset.h"

namespace sugoma
{
	using AssetMetaDrawFunc = std::function<bool(AssetMetadata*)>;
	template<typename T>
	struct EnumPair 
	{
		const char* name;
		T value;
	};
	template<typename T>
	T draw_enum_pairs(const char* label, EnumPair<T>* enums, uint32_t count, uint32_t selectedIndex)
	{
		T output = enums[selectedIndex].value;
		if (ImGui::BeginCombo(label, enums[selectedIndex].name)) 
		{
			for (uint32_t i = 0; i < count; ++i) 
			{
				bool selected = i == selectedIndex;
				auto& e = enums[i];
				if (ImGui::Selectable(e.name, &selected))
					output = e.value;
			}
			ImGui::EndCombo();
		}
		return output;
	};
	template<typename T>
	uint32_t index_of_enum_pair(EnumPair<T>* pairs, uint32_t count, T value) 
	{
		for (uint32_t i = 0; i < count; ++i) 
			if (pairs[i].value == value) return i;
		return 0;
	}
	template<typename T>
	bool draw_enum_pairs(const char* label, EnumPair<T>* enums, uint32_t count, T& value)
	{
		bool changed = false;
		uint32_t selectedIndex = index_of_enum_pair(enums, count, value);
		if (ImGui::BeginCombo(label, enums[selectedIndex].name))
		{
			for (uint32_t i = 0; i < count; ++i)
			{
				bool selected = i == selectedIndex;
				auto& e = enums[i];
				if (ImGui::Selectable(e.name, &selected)) {
					changed = (value != e.value);
					value = e.value;
				}
			}
			ImGui::EndCombo();
		}
		return changed;
	};

	bool draw_texture_meta(AssetMetadata* metadata) 
	{
		static EnumPair<TextureDimension> _tex_dim_enum[] =
		{
			{"Texture1D", TextureDimension::Texture1D},
			{"Texture2D", TextureDimension::Texture2D},
			{"Texture3D", TextureDimension::Texture3D},
			{"TextureCube", TextureDimension::TextureCube},
		};
		static EnumPair<TextureFilter> _tex_filter_enum[] =
		{
			{"Nearest", TextureFilter::NEAREST},
			{"Linear", TextureFilter::LINEAR},
			{"NearestMipmapNearest", TextureFilter::NEAREST_MIPMAP_NEAREST},
			{"LinearMipmapNearest", TextureFilter::LINEAR_MIPMAP_NEAREST},
			{"NearestMipmapLinear", TextureFilter::NEAREST_MIPMAP_LINEAR},
			{"LinearMipmapLinear", TextureFilter::LINEAR_MIPMAP_LINEAR}
		};
		static EnumPair<TextureWrapMode> _tex_wrap_mode[] =
		{
			{"Repeat", TextureWrapMode::REPEAT},
			{"MirroredRepeat", TextureWrapMode::MIRRORED_REPEAT},
			{"ClampToEdge", TextureWrapMode::CLAMP_TO_EDGE},
			{"ClampToBorder", TextureWrapMode::CLAMP_TO_BORDER},
		};
		static EnumPair<TextureFormat> _tex_format[] =
		{
			{"RED", TextureFormat::RED},
			{"RG", TextureFormat::RG},
			{"RGB", TextureFormat::RGB},
			{"BGR", TextureFormat::BGR},
			{"RGBA", TextureFormat::RGBA},
			{"BGRA", TextureFormat::BGRA},
			{"DEPTH_COMPONENT", TextureFormat::DEPTH_COMPONENT},
			{"DEPTH_STENCIL", TextureFormat::DEPTH_STENCIL},
			{"STENCIL_INDEX", TextureFormat::STENCIL_INDEX},
			{"LUMINANCE", TextureFormat::LUMINANCE},
			{"LUMINANCE_ALPHA", TextureFormat::LUMINANCE_ALPHA},
			{"R8", TextureFormat::R8},
			{"R16", TextureFormat::R16},
			{"RG8", TextureFormat::RG8},
			{"RG16", TextureFormat::RG16},
			{"R16F", TextureFormat::R16F},
			{"R32F", TextureFormat::R32F},
			{"RG16F", TextureFormat::RG16F},
			{"RG32F", TextureFormat::RG32F},
			{"R8I", TextureFormat::R8I},
			{"R8UI", TextureFormat::R8UI},
			{"R16I", TextureFormat::R16I},
			{"R16UI", TextureFormat::R16UI},
			{"R32I", TextureFormat::R32I},
			{"R32UI", TextureFormat::R32UI},
			{"RG8I", TextureFormat::RG8I},
			{"RG8UI", TextureFormat::RG8UI},
			{"RG16I", TextureFormat::RG16I},
			{"RG16UI", TextureFormat::RG16UI},
			{"RG32I", TextureFormat::RG32I},
			{"RG32UI", TextureFormat::RG32UI},
			{"RGB8", TextureFormat::RGB8},
			{"RGB16", TextureFormat::RGB16},
			{"RGB16F", TextureFormat::RGB16F},
			{"RGB32F", TextureFormat::RGB32F},
			{"RGB8I", TextureFormat::RGB8I},
			{"RGB8UI", TextureFormat::RGB8UI},
			{"RGB16I", TextureFormat::RGB16I},
			{"RGB16UI", TextureFormat::RGB16UI},
			{"RGB32I", TextureFormat::RGB32I},
			{"RGB32UI", TextureFormat::RGB32UI},
			{"RGBA8", TextureFormat::RGBA8},
			{"RGBA16", TextureFormat::RGBA16},
			{"RGBA16F", TextureFormat::RGBA16F},
			{"RGBA32F", TextureFormat::RGBA32F},
			{"RGBA8I", TextureFormat::RGBA8I},
			{"RGBA8UI", TextureFormat::RGBA8UI},
			{"RGBA16I", TextureFormat::RGBA16I},
			{"RGBA16UI", TextureFormat::RGBA16UI},
			{"RGBA32I", TextureFormat::RGBA32I},
			{"RGBA32UI", TextureFormat::RGBA32UI},
			{"SRGB", TextureFormat::SRGB},
			{"SRGBA", TextureFormat::SRGBA},
			{"SRGB8", TextureFormat::SRGB8},
			{"SRGBA8", TextureFormat::SRGBA8}
		};


		TextureMetadata& meta = *reinterpret_cast<TextureMetadata*>(metadata);
		bool changed = 
		draw_enum_pairs("Dimension", _tex_dim_enum, sizeof(_tex_dim_enum) / sizeof(_tex_dim_enum[0]), meta.dimension) +
		draw_enum_pairs("Mag. Filter", _tex_filter_enum, sizeof(_tex_filter_enum) / sizeof(_tex_filter_enum[0]), meta.magFilter) +
		draw_enum_pairs("Min. Filter", _tex_filter_enum, sizeof(_tex_filter_enum) / sizeof(_tex_filter_enum[0]), meta.minFilter) +
		draw_enum_pairs("Vertical Wrap", _tex_wrap_mode, sizeof(_tex_wrap_mode) / sizeof(_tex_wrap_mode[0]), meta.verticalWrap) +
		draw_enum_pairs("Horizontal Wrap", _tex_wrap_mode, sizeof(_tex_wrap_mode) / sizeof(_tex_wrap_mode[0]), meta.horizontalWrap) +
		draw_enum_pairs("Format", _tex_format, sizeof(_tex_format) / sizeof(_tex_format[0]), meta.textureFormat) +
		ImGui::Checkbox("HDR", &meta.hdr) +
		ImGui::Checkbox("Flip", &meta.flip);
		return changed;
	}


	std::unordered_map<AssetType, AssetMetaDrawFunc> meta_draw_funcs
	{
		{(AssetType)AssetTypes::Texture, draw_texture_meta}
	};



	AssetPacker::AssetPacker() : DebugWindow("Asset Packer Window") { m_flags = ImGuiWindowFlags_MenuBar; }
	AssetPacker::~AssetPacker()
	{

	}
	void AssetPacker::OnCreate()
	{
		OpenPath("data");
	}
	void AssetPacker::OnClose()
	{
		ClearEntries();
	}
	void AssetPacker::OnGUI()
	{
		if (ImGui::BeginMenuBar()) 
		{
			if (ImGui::BeginMenu("Export")) 
			{
				if (ImGui::MenuItem("Export SAF"))
					ExportSAF((m_active_path.parent_path() / m_active_path.filename().stem()).string() + ".saf");
				if (ImGui::MenuItem("Export SAF as")) 
				{
					std::filesystem::path path = FileUtility::SaveFileDialog(m_active_path.parent_path(), { {L"Sugoma Asset File", L".saf"} }, m_active_path.filename().stem().wstring(), L"saf");
					ExportSAF(path);
				}

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		ImVec2 tableSize = ImGui::GetContentRegionAvail();
		if (ImGui::BeginTable("##main", 3, ImGuiTableFlags_Resizable, tableSize))
		{
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			DrawEntryHierarchy();
			ImGui::TableSetColumnIndex(1);
			DrawEntryMetadata();
			ImGui::TableSetColumnIndex(2);
			DrawEntryPreview();
			ImGui::EndTable();
		}
	}


	void clear_entry(APFileEntry& entry) 
	{
		if (entry.metadata) delete entry.metadata;
		for (auto& subentry : entry.children) clear_entry(subentry);
	}

	void AssetPacker::ClearEntries()
	{
		clear_entry(m_active);
		m_active = {};
		m_selected = nullptr;
	}
	void AssetPacker::RegenEntries()
	{
		m_active = LoadEntry(m_active_path);
	}
	void AssetPacker::OpenPath(const std::filesystem::path& path)
	{
		m_active_path = path;
		RegenEntries();
	}
	APFileEntry AssetPacker::LoadEntry(const std::filesystem::path& path)
	{
		APFileEntry entry;
		entry.path = path;
		entry.extension = path.filename().extension().string();
		entry.is_directory = std::filesystem::is_directory(path);
		entry.assetType = Assets::GetTypeFromExtension(entry.extension);
		entry.name = path.filename().stem().string();
		if (entry.assetType != 0) 
		{
			auto info = Assets::GetAssetTypeInfo(entry.assetType);
			entry.assetTypeName = entry.assetTypeName = info->assetTypeName;
			std::filesystem::path meta_p = path.string() + ".meta";
			if (std::filesystem::exists(meta_p)) 
			{
				entry.included = true;
				entry.metadata = info->createMetadata();

				std::ifstream stream(meta_p);
				std::string hex_guid;
				stream >> hex_guid;
				entry.metadata->assetGuid = hex_guid;
				entry.hexGUID = entry.metadata->assetGuid.ToString();
				entry.metadata->Deserialize(stream);
				stream.close();
			}
			else 
			{
				entry.included = false;
			}
		}
		if (entry.is_directory) 
		{
			auto iterator = std::filesystem::directory_iterator(path);
			for (auto file : iterator) 
				entry.children.push_back(LoadEntry(file));
		}
		return entry;
	}
	void AssetPacker::DrawDirectoryEntry(APFileEntry& entry)
	{
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		if (ImGui::ArrowButton(entry.name.c_str(), entry.opened ? ImGuiDir_Down : ImGuiDir_Right)) entry.opened = !entry.opened;
		ImGui::SameLine();
		ImGui::Text(entry.name.c_str());
		if(entry.opened)
		{
			ImGui::Indent(5);
			for (auto& e : entry.children) 
			{
				if (e.is_directory) DrawDirectoryEntry(e);
				else if(e.assetType != 0) DrawEntry(e);
			}
			ImGui::Unindent(5);
		}
	}
	void AssetPacker::DrawEntry(APFileEntry& entry)
	{
		ImGui::TableNextRow();
		bool selected = &entry == m_selected;
		if (!entry.included) ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.7f, 0.7f, 1.0f));
		ImGui::TableSetColumnIndex(0);
		if (ImGui::Selectable(entry.name.c_str(), &selected, ImGuiSelectableFlags_SpanAllColumns)) SetSelectedEntry(entry);
		ImGui::TableSetColumnIndex(1);
		ImGui::Text(entry.assetTypeName);
		if (!entry.included) ImGui::PopStyleColor();
	}
	void AssetPacker::SetSelectedEntry(APFileEntry& entry)
	{
		m_selected = &entry;
		ReloadPreview();
	}
	void AssetPacker::DrawEntryHierarchy()
	{
		if (ImGui::BeginTable("##entry_hierarchy", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg))
		{
			ImGui::TableSetupColumn("Entry");
			ImGui::TableSetupColumn("Type");
			ImGui::TableHeadersRow();

			DrawDirectoryEntry(m_active);

			ImGui::EndTable();
		}
	}
	void AssetPacker::DrawEntryMetadata()
	{
		if (m_selected == nullptr) return;
		if (!m_selected->included) 
		{
			if (ImGui::Button("Include")) 
			{
				auto info = Assets::GetAssetTypeInfo(m_selected->assetType);
				m_selected->metadata = info->createMetadata();
				m_selected->metadata->assetGuid = GUID::NewGUID();
				m_selected->hexGUID = m_selected->metadata->assetGuid.ToString();
				m_selected->included = true;
			}
			return;
		}
		std::string str = std::format("GUID : {0}", m_selected->hexGUID.c_str());
		if (ImGui::Selectable(str.c_str()))
			glfwSetClipboardString(glfwGetCurrentContext(), m_selected->hexGUID.c_str());
		auto it = meta_draw_funcs.find(m_selected->assetType);
		bool reload_preview = false;
		if (it != meta_draw_funcs.end())
			reload_preview = it->second(m_selected->metadata);
		if (ImGui::Button("Save"))
			SaveSelectedMeta();
		if (reload_preview)
			ReloadPreview();
	}
	void AssetPacker::DrawEntryPreview()
	{
		if (!m_selected || !m_selected->included) return;
		if (!m_preview) ReloadPreview();


		switch (m_selected->assetType)
		{
		case (AssetType)AssetTypes::Texture:
		{
			Ref<Texture> texture = m_preview;
			ImVec2 avail = ImGui::GetContentRegionAvail();
			float avail_aspect = avail.x / avail.y;
			float tex_aspect = (float)texture->Width() / texture->Height();
			if (tex_aspect > avail_aspect) avail.y = avail.x / tex_aspect;
			else if (tex_aspect < avail_aspect) avail.x = avail.y * tex_aspect;
			ImGui::Image(texture->Handle(), avail, ImVec2(0, 1), ImVec2(1, 0));
			break;
		}
		default:
			break;
		}
	}
	void AssetPacker::SaveSelectedMeta()
	{
		std::filesystem::path path = m_selected->path.string() + ".meta";
		std::ofstream stream(path);
		stream << m_selected->metadata->assetGuid.ToString() << '\n';
		m_selected->metadata->Serialize(stream);
		stream.close();
	}
	void AssetPacker::ReloadPreview()
	{
		if (!m_selected) return;
		if (m_preview)
			Resources::Destroy(m_preview);
		m_preview = 0;
		switch (m_selected->assetType)
		{
		case (AssetType)AssetTypes::Texture:
			{
			auto info = Assets::GetAssetTypeInfo(m_selected->assetType);
			Buffer b = info->packFile(m_selected->path, m_selected->metadata);
			m_preview = info->unpackAsset(b);
			b.Free();
			break;
			}
		default:
			break;
		}
	}

	void BuildSAF(const APFileEntry& entry, std::vector<SAFEntryHeader>& headers, std::vector<Buffer>& buffers, bool root = false) 
	{
		if (!entry.is_directory && !entry.included)
			return;
		uint32_t idx = headers.size();
		if (!root) {
			headers.emplace_back();
			buffers.emplace_back();
		}
		if (entry.is_directory) 
		{
			uint32_t count = 0;
			for (auto& child : entry.children) {
				if (!child.is_directory && !child.included) continue;
				BuildSAF(child, headers, buffers);
				++count;
			}
			if (root) return;
			auto& e = headers[idx];
			e.size = count;
		}
		else 
		{
			auto& e = headers[idx];
			auto info = Assets::GetAssetTypeInfo(entry.assetType);
			Buffer b = info->packFile(entry.path, entry.metadata);
			buffers[idx] = b;
			e.size = b.Size();
			e.guid = entry.metadata->assetGuid;
		}
		auto& e = headers[idx];
		e.type = entry.assetType;
		e.name = entry.name;
	}

	void AssetPacker::ExportSAF(const std::filesystem::path& destination)
	{
		SAFInfo info{};
		info.header.nameOffset = 0;
		info.header.sizeOffset = 0;

		std::vector<SAFEntryHeader>& entries = info.entries;
		std::vector<Buffer> buffers;
		BuildSAF(m_active, entries, buffers, true);
		SAFUtility::WriteSAF(destination, info, buffers);
		for (auto& b : buffers) b.Free();
	}
}