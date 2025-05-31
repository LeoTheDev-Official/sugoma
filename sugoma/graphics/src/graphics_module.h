#pragma once
#include "engine/module.h"
namespace sugoma::graphics 
{
	enum GraphicsMouleSteps
	{
		GraphicsModuleStep_PollEvents = 0,
		GraphicsModuleStep_Clear = 1,
		GraphicsModuleStep_Present = 31,
	};
	class Window;
	using namespace sugoma::core;
	class GraphicsModule : public Module
	{
	public:
		GraphicsModule();
		~GraphicsModule();
		void OnAttach() override;
		void OnDetach() override;
		
		void PollEvents();
		void ClearWindow();
		void Present();
		void EventCallback(Event& event);

		void OnEvent(Event& event) override;
		bool Validate() override;

		Window* GetWindow() const;
	private:
		Window* m_window;
	};
}