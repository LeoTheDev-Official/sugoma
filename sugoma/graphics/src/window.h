#pragma once
#include "events/event.h"
#include <cstdint>
#include <glm/glm.hpp>
#include <functional>

struct GLFWwindow;
namespace sugoma::graphics 
{
	using namespace sugoma::core;
	bool __initialize_glfw();
	bool __glfw_initialized();
	class Framebuffer;
	using WindowEventCallbackFunc = std::function<void(Event&)>;
	struct WindowCreateInfo 
	{
		glm::uvec2 size = {800, 600};
		glm::uvec2 position = {-1, -1};
		const char* title = "Default Window";
		bool resizable = true;
		bool vSync = true;
	};
	class Window
	{
	public:
		Window() = delete;
		Window(const WindowCreateInfo& createInfo);
		void MakeContextCurrent();
		void SetTitle(const char* title);
		void SetSize(uint32_t width, uint32_t height);
		void SetPosition(uint32_t x, uint32_t y);
		glm::uvec2 GetSize() const;
		glm::uvec2 GetPosition() const;
		GLFWwindow* GetHandle() const;
	public:
		void PollEvents();
		void Clear(glm::vec4 color);
		void Data(Framebuffer* source, uint32_t attachment = 0, uint32_t filter = 9729);
		void Present();
	public:
		WindowEventCallbackFunc event_callback;
	private:
		void OnEvent(Event& evt);
		GLFWwindow* m_window_handle = nullptr;
	};
	float glfwTime();
	Window* GraphicsContext();
}