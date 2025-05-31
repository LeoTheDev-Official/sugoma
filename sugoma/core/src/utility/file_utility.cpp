#include <shobjidl.h>
#include <fstream>
#include "file_utility.h"
#include "debug/logger.h"

namespace sugoma::core 
{
	std::string FileUtility::ReadStringFile(const std::filesystem::path& path)
	{
		std::ifstream stream(path);
		if (!stream.good()) { sugoma_error("File at path ( " << path << " ) empty or does not exist."); return ""; }
		std::stringstream sstr;
		sstr << stream.rdbuf();
		stream.close();
		return sstr.str();
	}

    std::filesystem::path FileUtility::OpenFolderDialog(const std::filesystem::path& startIn) {
        std::filesystem::path result;
        (void)CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

        if (IFileDialog* dlg; SUCCEEDED(CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&dlg)))) {
            dlg->SetOptions(FOS_PICKFOLDERS | FOS_FORCEFILESYSTEM);

            // Set starting folder
            if (!startIn.empty()) {
                IShellItem* folderItem = nullptr;
                if (SUCCEEDED(SHCreateItemFromParsingName(startIn.c_str(), nullptr, IID_PPV_ARGS(&folderItem)))) {
                    dlg->SetFolder(folderItem);
                    folderItem->Release();
                }
            }

            if (SUCCEEDED(dlg->Show(nullptr))) {
                if (IShellItem* item; SUCCEEDED(dlg->GetResult(&item))) {
                    PWSTR path;
                    if (SUCCEEDED(item->GetDisplayName(SIGDN_FILESYSPATH, &path))) {
                        result = path;
                        CoTaskMemFree(path);
                    }
                    item->Release();
                }
            }

            dlg->Release();
        }

        CoUninitialize();
        return result;
    }
    std::filesystem::path FileUtility::OpenFileDialog(const std::filesystem::path& startIn,
        const std::vector<std::pair<std::wstring, std::wstring>>& filters)
    {
        std::filesystem::path result;
        (void)CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

        if (IFileDialog* dlg; SUCCEEDED(CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&dlg)))) {
            // Convert filters to COMDLG_FILTERSPEC
            std::vector<COMDLG_FILTERSPEC> specs;
            for (const auto& [desc, ext] : filters) {
                specs.push_back({ desc.c_str(), ext.c_str() });
            }
            if (!specs.empty())
                dlg->SetFileTypes(static_cast<UINT>(specs.size()), specs.data());

            // Set starting folder
            if (!startIn.empty()) {
                IShellItem* folderItem = nullptr;
                if (SUCCEEDED(SHCreateItemFromParsingName(startIn.c_str(), nullptr, IID_PPV_ARGS(&folderItem)))) {
                    dlg->SetFolder(folderItem);
                    folderItem->Release();
                }
            }

            if (SUCCEEDED(dlg->Show(nullptr))) {
                if (IShellItem* item; SUCCEEDED(dlg->GetResult(&item))) {
                    PWSTR path;
                    if (SUCCEEDED(item->GetDisplayName(SIGDN_FILESYSPATH, &path))) {
                        result = path;
                        CoTaskMemFree(path);
                    }
                    item->Release();
                }
            }

            dlg->Release();
        }

        CoUninitialize();
        return result;
    }
    std::filesystem::path FileUtility::SaveFileDialog(
        const std::filesystem::path& startIn,
        const std::vector<std::pair<std::wstring, std::wstring>>& filters,
        const std::wstring& defaultName,
        const std::wstring& defaultExtension
    ) {
        std::filesystem::path result;
        (void)CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

        if (IFileDialog* dlg; SUCCEEDED(CoCreateInstance(CLSID_FileSaveDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&dlg)))) {
            // Set file types
            std::vector<COMDLG_FILTERSPEC> specs;
            for (const auto& [desc, ext] : filters) {
                specs.push_back({ desc.c_str(), ext.c_str() });
            }
            if (!specs.empty())
                dlg->SetFileTypes(static_cast<UINT>(specs.size()), specs.data());

            dlg->SetDefaultExtension(defaultExtension.c_str());
            dlg->SetFileName(defaultName.c_str());

            // Set initial directory
            if (!startIn.empty()) {
                IShellItem* folderItem = nullptr;
                if (SUCCEEDED(SHCreateItemFromParsingName(startIn.c_str(), nullptr, IID_PPV_ARGS(&folderItem)))) {
                    dlg->SetFolder(folderItem);
                    folderItem->Release();
                }
            }

            if (SUCCEEDED(dlg->Show(nullptr))) {
                if (IShellItem* item; SUCCEEDED(dlg->GetResult(&item))) {
                    PWSTR path;
                    if (SUCCEEDED(item->GetDisplayName(SIGDN_FILESYSPATH, &path))) {
                        result = path;
                        CoTaskMemFree(path);
                    }
                    item->Release();
                }
            }

            dlg->Release();
        }

        CoUninitialize();
        return result;
    }
    

}