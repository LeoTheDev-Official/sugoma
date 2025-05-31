#pragma once
#include <string>
#include <filesystem>
namespace sugoma::core 
{
	class FileUtility 
	{
	public:
		static std::string ReadStringFile(const std::filesystem::path& path);
		static std::filesystem::path OpenFolderDialog(const std::filesystem::path& startIn = {});
		static std::filesystem::path OpenFileDialog(const std::filesystem::path& startIn = {}, const std::vector<std::pair<std::wstring, std::wstring>>& filters = { {L"All Files", L"*.*"} });
		static std::filesystem::path SaveFileDialog(const std::filesystem::path& startIn = {}, const std::vector<std::pair<std::wstring, std::wstring>>& filters = { {L"All Files", L"*.*"} }, const std::wstring& defaultName = L"untitled", const std::wstring& defaultExtension = L"txt");
	};
}