#pragma once
#include "events/event.h"
#include <cstdint>
#include <glm/glm.hpp>
#include <functional>
#include "graphics_device.h"
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
		void SetEventCallback(WindowEventCallbackFunc func);
		glm::uvec2 GetSize() const;
		glm::uvec2 GetPosition() const;
		GLFWwindow* GetHandle() const;
		const GraphicsDevice& Device() const;
	public:
		void PollEvents();
		void Clear(glm::vec4 color);
		void Data(const Framebuffer* source, uint32_t attachment = 0, uint32_t filter = 9729);
		void Present();
		void SetCursorLock(bool locked);
	private:
		void OnEvent(Event& evt);
		WindowEventCallbackFunc m_event_callback;
		GLFWwindow* m_window_handle = nullptr;
		GraphicsDevice m_device;
	};
	float glfwTime();
	Window* GraphicsContext();
}