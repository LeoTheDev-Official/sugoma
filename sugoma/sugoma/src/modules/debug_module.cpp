#include "debug_module.h"

#include "imgui.h"


#include "tools/asset_packer.h"
#include "tools/audio_debug.h"


namespace sugoma 
{
	DebugModule::DebugModule() : Module("DdebugModule") {}
	void DebugModule::OnAttach()
	{

	}
	void DebugModule::OnDetach()
	{

	}
	void DebugModule::Tick()
	{
		static std::vector<DebugWindow*> windows_closing;
		for (auto window : m_windows) 
		{
			if (!window->m_open) {
				windows_closing.push_back(window);
				continue;
			}
			if (ImGui::Begin(window->Title().c_str(), &window->m_open, window->m_flags)) {
				window->OnGUI();
				ImGui::End();

			}
		}
		for (auto w : windows_closing) 
		{
			auto it = std::find(m_windows.begin(), m_windows.end(), w);
			m_windows.erase(it);
			w->OnClose();
			delete w;
		}
		windows_closing.clear();



		if (ImGui::BeginMainMenuBar()) 
		{
			if (ImGui::BeginMenu("File")) 
			{
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Tools"))
			{
				if (ImGui::MenuItem("Asset Packer")) GetWindow<AssetPacker>();
				if (ImGui::MenuItem("Audio Debug")) GetWindow<AudioDebugWindow>();
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Help"))
			{
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
	}
	bool DebugModule::Validate() { return true; }
	DebugWindow::DebugWindow(const char* title) : m_title(title) {}
	DebugWindow::~DebugWindow() {}
	void DebugWindow::Close() { m_open = false; }
	const std::string& DebugWindow::Title() const { return m_title; }
}