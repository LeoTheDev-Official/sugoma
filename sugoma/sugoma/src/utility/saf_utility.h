#pragma once
#include <string>
#include <unordered_map>
#include <filesystem>
#include "assets/asset.h"
namespace sugoma 
{
	struct SAFHeader 
	{
		int32_t sizeOffset;
		int8_t nameOffset;
	};
	struct SAFEntryHeader 
	{
		std::string name;
		std::string fullName;
		AssetType type = 0; //0 = directory
		uint32_t flags = 0;
		uint32_t size = 0; //in case of a dictionary this determines the number of entries
		uint32_t offset = 0;
		GUID guid;
	};
	struct SAFInfo 
	{
		SAFHeader header;
		std::vector<SAFEntryHeader> entries;
	};
	class SAFUtility 
	{
	public:
		static bool ReadSAF(const std::filesystem::path& path, SAFInfo& output);
		static bool WriteSAF(const std::filesystem::path& path, const SAFInfo& file, const std::vector<Buffer>& buffers);
	};
}