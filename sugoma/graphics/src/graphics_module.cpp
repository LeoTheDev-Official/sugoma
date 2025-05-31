#include "graphics_module.h"
#include "window.h"
#include "debug/logger.h"

#include "engine/engine.h"
#include "GL/glew.h"

#include "pipeline/compute_pipeline.h"
#include "textures/texture2D.h"
#include "utility/file_utility.h"

namespace sugoma::graphics 
{
	GraphicsModule::GraphicsModule() : Module("GraphicsModule")
	{
		PushStepFunction(GraphicsMouleSteps::GraphicsModuleStep_PollEvents, std::bind(&GraphicsModule::PollEvents, this));
		PushStepFunction(GraphicsMouleSteps::GraphicsModuleStep_Clear, std::bind(&GraphicsModule::ClearWindow, this));
		PushStepFunction(GraphicsMouleSteps::GraphicsModuleStep_Present, std::bind(&GraphicsModule::Present, this));
	}
	GraphicsModule::~GraphicsModule()
	{

	}
	void GraphicsModule::OnAttach()
	{
		auto& einfo = Context()->Info();

		WindowCreateInfo createInfo{};
		createInfo.size = { 1280, 720 };
		createInfo.position = { -1, -1 };
		createInfo.title = einfo.appName;
		createInfo.resizable = true;
		createInfo.vSync = false;
		
		m_window = new Window(createInfo);
		m_window->MakeContextCurrent();
		m_window->event_callback = std::bind(&GraphicsModule::EventCallback, this, std::placeholders::_1);

		const GLubyte* renderer = glGetString(GL_RENDERER);
		const GLubyte* vendor = glGetString(GL_VENDOR);
		const GLubyte* version = glGetString(GL_VERSION);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		sugoma_info("OpenGL Initialized. Device : " << renderer << " " << vendor << " " << version);
	}
	void GraphicsModule::OnDetach()
	{
		if (m_window) { delete m_window; m_window = nullptr; }
	}
	void GraphicsModule::PollEvents()
	{
		m_window->PollEvents();
	}
	void GraphicsModule::ClearWindow()
	{
		m_window->Clear(glm::vec4(0.05f));
	}
	void GraphicsModule::Present()
	{
		m_window->Present();
	}
	void GraphicsModule::EventCallback(Event& event)
	{
		Context()->SendEvent(event);
	}
	void GraphicsModule::OnEvent(Event& event)
	{

	}
	bool GraphicsModule::Validate()
	{
		if (!__glfw_initialized())
		{
			if (!__initialize_glfw())
			{
				sugoma_error("Failed to initialize GLFW");
				return false;
			}
		}
		return true;
	}
	Window* GraphicsModule::GetWindow() const { return m_window; }
}