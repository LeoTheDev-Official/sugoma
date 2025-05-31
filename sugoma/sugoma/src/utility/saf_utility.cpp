#include "saf_utility.h"
#include "debug/logger.h"
#include <fstream>


namespace sugoma
{
	using namespace sugoma::core;
	#pragma pack(1)
	struct saf_header
	{
		char key[4];
		int32_t size_offset;
		int8_t name_offset;
	};
	struct saf_entry_header
	{
		char name[32];
		uint8_t guid[sizeof(GUID)];
		int16_t type = 0; //0 = directory
		uint32_t flags = 0;
		uint32_t size = 0; //in case of a dictionary this determines the number of entries
	};





	static bool __validate_saf_header(const saf_header& header, const std::filesystem::path& path) 
	{
		std::string_view a(header.key, 4);
		if (a != "$SAF") { sugoma_error(path << " is not a valid SAF(Sugoma Asset File)"); return false; }
		return true;
	}
	static void __iterate_saf_directory_offsets(const std::string& pname, uint32_t& pindex, uint32_t count, uint32_t& cursor, SAFInfo& info) 
	{
		for (uint32_t i = 0; ((i < count) && (pindex < info.entries.size())); ++i)
		{
			cursor += sizeof(saf_entry_header);
			auto& entry = info.entries[pindex];
			entry.fullName = (pname.empty()?"":(pname + "/")) + entry.name;
			entry.offset = cursor;
			if (entry.type == 0) __iterate_saf_directory_offsets(entry.fullName, ++pindex, entry.size, cursor, info);
			else { cursor += entry.size; ++pindex; }
		}
	}
    bool SAFUtility::ReadSAF(const std::filesystem::path& path, SAFInfo& output)
    {
		std::ifstream stream(path, std::ios::binary);
		size_t size = std::filesystem::file_size(path);
		if (!stream.is_open()) { sugoma_error("Could not open file : " << path); return false; }
		saf_header header;
		stream.read(reinterpret_cast<char*>(&header), sizeof(saf_header));
		if (!__validate_saf_header(header, path)) { stream.close(); return false; }

		output.header.nameOffset = header.name_offset;
		output.header.sizeOffset = header.size_offset;

		saf_entry_header entry{};
		SAFEntryHeader o_entry{};
		while (stream.tellg() < size)
		{
			stream.read(reinterpret_cast<char*>(&entry), sizeof(saf_entry_header));
			for (uint8_t i = 0; i < sizeof(entry.name); ++i) entry.name[i] -= header.name_offset;
			entry.size -= header.size_offset;
			o_entry.name = std::string(entry.name);
			o_entry.type = entry.type;
			o_entry.size = entry.size;
			o_entry.flags = entry.flags;
			o_entry.guid = entry.guid;

			if (entry.type != 0) stream.seekg(stream.tellg() + (std::streamoff)entry.size);

			output.entries.push_back(o_entry);
		}
		stream.close();

		uint32_t cursor = sizeof(saf_header);
		uint32_t index = 0;
		__iterate_saf_directory_offsets("", index, output.entries.size(), cursor, output);

		return true;
    }
	bool SAFUtility::WriteSAF(const std::filesystem::path& path, const SAFInfo& file, const std::vector<Buffer>& buffers)
	{
		std::ofstream stream(path, std::ios::binary);
		saf_header header{};
		memcpy(header.key, "$SAF", 4);
		header.name_offset = file.header.nameOffset;
		header.size_offset = file.header.sizeOffset;

		stream.write(reinterpret_cast<char*>(&header), sizeof(saf_header));

		saf_entry_header entry{};
		for (uint32_t i = 0; i < file.entries.size(); ++i) 
		{
			auto& e = file.entries[i];
			memset(entry.name, 0, sizeof(entry.name));
			memcpy(entry.name, e.name.data(), std::min(sizeof(entry.name) - 1, e.name.length()));
			memcpy(entry.guid, &e.guid, sizeof(GUID));
			for (uint8_t i = 0; i < sizeof(entry.name); ++i) entry.name[i] += header.name_offset;
			entry.type = e.type;
			entry.flags = e.flags;
			entry.size = e.size + header.size_offset;
			stream.write(reinterpret_cast<char*>(&entry), sizeof(entry));
			if(entry.type != 0) stream.write(reinterpret_cast<const char*>(buffers[i].Data()), e.size);
		}
		stream.close();
		return true;
	}
}
