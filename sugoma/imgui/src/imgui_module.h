#pragma once
#include "engine/module.h"
namespace sugoma::imgui
{
	enum ImGuiModuleSteps
	{
		ImGuiModuleStep_Begin = 15,
		ImGuiModuleStep_End = 30
	};
	using namespace core;
	class ImGuiModule : public Module
	{
	public:
		ImGuiModule();
		~ImGuiModule();
		void OnAttach() override;
		void OnDetach() override;
		bool Validate() override;

		void BeginFrame();
		void EndFrame();
	private:
	};
}