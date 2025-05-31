#include "imgui_module.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"

#include "window.h"

namespace sugoma::imgui 
{
	static bool __imgui_initialized = false;
	static bool __initialize_imgui() 
	{
		if (__imgui_initialized)
			return true;
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsLight();

		ImGui_ImplGlfw_InitForOpenGL(graphics::GraphicsContext()->GetHandle(), true);
		const char* glsl_version = "#version 130";
		ImGui_ImplOpenGL3_Init(glsl_version);
		__imgui_initialized = true;
		return true;
	}
	static void __shutdown_imgui() 
	{
		if (!__imgui_initialized)
			return;
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		__imgui_initialized = false;
	}


	ImGuiModule::ImGuiModule() : Module("ImGuiModule")
	{	
		PushStepFunction(ImGuiModuleSteps::ImGuiModuleStep_Begin, std::bind(&ImGuiModule::BeginFrame, this));
		PushStepFunction(ImGuiModuleSteps::ImGuiModuleStep_End, std::bind(&ImGuiModule::EndFrame, this));
	}
	ImGuiModule::~ImGuiModule()
	{
	}
	void ImGuiModule::OnAttach()
	{
		__initialize_imgui();
	}
	void ImGuiModule::OnDetach()
	{
		__shutdown_imgui();
	}
	bool ImGuiModule::Validate()
	{
		return true;
	}
	void ImGuiModule::BeginFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}
	void ImGuiModule::EndFrame()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		auto io = ImGui::GetIO();
		if (io.ConfigFlags * ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* window = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(window);
		}
	}
}