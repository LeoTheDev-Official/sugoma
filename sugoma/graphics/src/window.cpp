#include <GL/glew.h>
#include <glfw/glfw3.h>
#include "window.h"
#include "debug/logger.h"

#include "events/window_events.h"
#include "events/input_events.h"

#include "buffers/framebuffer.h"
namespace sugoma::graphics
{
	static bool s_glfw_initialized = false;
	static Window* s_active_context;
	void GLAPIENTRY __send_gl_message(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
	{
		const char* src_str;
		switch (source)
		{
		case GL_DEBUG_SOURCE_API:
			src_str = "API";
			break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
			src_str = "WINDOW SYSTEM";
			break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER:
			src_str = "SHADER COMPILER";
			break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:
			src_str = "THIRD PARTY";
			break;
		case GL_DEBUG_SOURCE_APPLICATION:
			src_str = "APPLICATION";
			break;
		case GL_DEBUG_SOURCE_OTHER:
			src_str = "OTHER";
			break;
		default:
			src_str = "UNKNOWN";
			break;
		}
		const char* type_str;
		switch (type)
		{
		case GL_DEBUG_TYPE_ERROR:
			type_str = "ERROR";
			break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			type_str = "DEPRECATED BEHAVIOR";
			break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			type_str = "UNDEFINED BEHAVIOR";
			break;
		case GL_DEBUG_TYPE_PORTABILITY:
			type_str = "PORTABILITY";
			break;
		case GL_DEBUG_TYPE_PERFORMANCE:
			type_str = "PERFORMANCE";
			break;
		case GL_DEBUG_TYPE_MARKER:
			type_str = "MARKER";
			break;
		case GL_DEBUG_TYPE_PUSH_GROUP:
			type_str = "PUSH GROUP";
			break;
		case GL_DEBUG_TYPE_POP_GROUP:
			type_str = "POP GROUP";
			break;
		case GL_DEBUG_TYPE_OTHER:
			type_str = "OTHER";
			break;
		default:
			type_str = "UNKNOWN";
			break;
		}
		std::stringstream msg;
		msg << "<OpenGL : " << src_str << " : " << type_str << " : " << id << "> " << message;
		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:
			sugoma_error(msg.str());
			break;
		case GL_DEBUG_SEVERITY_MEDIUM:
			sugoma_warning(msg.str());
			break;
		case GL_DEBUG_SEVERITY_LOW:
			sugoma_log(msg.str());
			break;
		//case GL_DEBUG_SEVERITY_NOTIFICATION:
			//sugoma_info(msg.str());
			//break;
		}
	}


	bool __initialize_glfw()
	{
		if (s_glfw_initialized) return true;
		int glfw_code = glfwInit();
		sugoma_assert(glfw_code == GLFW_TRUE, "GLFW initialization failed.");
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef _DEBUG
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
		s_glfw_initialized = true;
		return true;
	}
	bool __glfw_initialized() { return s_glfw_initialized; }

	float glfwTime() { return glfwGetTime(); }

	Window* GraphicsContext()
	{
		return s_active_context;
	}

	Window::Window(const WindowCreateInfo& createInfo)
	{
		sugoma_assert(__glfw_initialized(), "GLFW must be initialized before creating a Window object.");
		glfwWindowHint(GLFW_RESIZABLE, createInfo.resizable);
		m_window_handle = glfwCreateWindow(createInfo.size.x, createInfo.size.y, createInfo.title, nullptr, nullptr);
		sugoma_assert(m_window_handle, "Window creation failed.");
		glfwMakeContextCurrent(m_window_handle);
		glfwSetWindowUserPointer(m_window_handle, this);
		glfwSwapInterval(createInfo.vSync);
		int glew_code = glewInit();
		sugoma_assert(glew_code == GLEW_OK, "GLEW initialization failed.");
#ifdef _DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(__send_gl_message, nullptr);
#endif
		if (createInfo.position.x != -1) glfwSetWindowPos(m_window_handle, createInfo.position.x, createInfo.position.y);

		m_device.Fetch();

		glfwSetWindowCloseCallback(m_window_handle, [](GLFWwindow* window)
			{
				auto win = static_cast<Window*>(glfwGetWindowUserPointer(window));
				WindowCloseEvent evt;
				win->OnEvent(evt);
			});
		glfwSetWindowPosCallback(m_window_handle, [](GLFWwindow* window, int x, int y)
			{
				auto win = static_cast<Window*>(glfwGetWindowUserPointer(window));
				WindowMoveEvent evt(x, y);
				win->OnEvent(evt);
			});
		glfwSetWindowSizeCallback(m_window_handle, [](GLFWwindow* window, int width, int height)
			{
				auto win = static_cast<Window*>(glfwGetWindowUserPointer(window));
				WindowResizeEvent evt(width, height);
				win->OnEvent(evt);
			});
		glfwSetWindowFocusCallback(m_window_handle, [](GLFWwindow* window, int focused)
			{
				auto win = static_cast<Window*>(glfwGetWindowUserPointer(window));
				WindowFocusEvent evt(focused == GLFW_TRUE);
				win->OnEvent(evt);
			});
		
		glfwSetKeyCallback(m_window_handle, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				auto win = static_cast<Window*>(glfwGetWindowUserPointer(window));
				switch (action)
				{
				case GLFW_PRESS: {
					KeyDownEvent evt(key);
					win->OnEvent(evt);
					break;
				}
				case GLFW_RELEASE: {
					KeyUpEvent evt(key);
					win->OnEvent(evt);
					break;
				}
				case GLFW_REPEAT: {
					KeyRepeatEvent evt(key);
					win->OnEvent(evt);
					break;
				}
				}
			});
		glfwSetCursorPosCallback(m_window_handle, [](GLFWwindow* window, double x, double y)
			{
				auto win = static_cast<Window*>(glfwGetWindowUserPointer(window));
				MouseMovedEvent evt({ (int)x, (int)y });
				win->OnEvent(evt);
			});
		glfwSetMouseButtonCallback(m_window_handle, [](GLFWwindow* window, int button, int action, int mods)
			{
				auto win = static_cast<Window*>(glfwGetWindowUserPointer(window));
				switch (action)
				{
				case GLFW_PRESS: {
					MouseButtonDownEvent evt(button);
					win->OnEvent(evt);
					break;
				}
				case GLFW_RELEASE: {
					MouseButtonUpEvent evt(button);
					win->OnEvent(evt);
					break;
				}
				}
			});
		glfwSetScrollCallback(m_window_handle, [](GLFWwindow* window, double xoffset, double yoffset)
			{
				auto win = static_cast<Window*>(glfwGetWindowUserPointer(window));
				MouseScrollEvent evt({ (int)xoffset, (int)yoffset });
				win->OnEvent(evt);
			});


	}
	void Window::MakeContextCurrent() { s_active_context = this; }
	void Window::SetTitle(const char* title) { glfwSetWindowTitle(m_window_handle, title); }
	void Window::SetSize(uint32_t width, uint32_t height) { glfwSetWindowSize(m_window_handle, width, height); }
	void Window::SetPosition(uint32_t x, uint32_t y) { glfwSetWindowPos(m_window_handle, x, y); }
	void Window::SetEventCallback(WindowEventCallbackFunc func) { m_event_callback = func; }
	glm::uvec2 Window::GetSize() const 
	{ 
		int w, h;
		glfwGetWindowSize(m_window_handle, &w, &h);
		return { w, h };
	}
	glm::uvec2 Window::GetPosition() const 
	{ 
		int x, y;
		glfwGetWindowPos(m_window_handle, &x, &y);
		return { x, y };
	}
	GLFWwindow* Window::GetHandle() const { return m_window_handle; }
	const GraphicsDevice& Window::Device() const { return m_device; }
	void Window::PollEvents() { glfwPollEvents(); }
	void Window::Clear(glm::vec4 color)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		auto size = GetSize();
		glViewport(0, 0, size.x, size.y);
		glClearColor(color.r, color.g, color.b, color.a);
		glClear(GL_COLOR_BUFFER_BIT);
	}
	void Window::Data(const Framebuffer* framebuffer, uint32_t attachment, GLEnum filter)
	{
		glm::uvec2 w_size = GetSize();
		auto spec = framebuffer->Specification();
		glm::uvec2 f_size = { spec.width, spec.height };

		glm::uvec2 dst_offset = { 0, 0 }, dst_size = w_size;

		float target_aspect = (float)f_size.x / f_size.y;
		float aspect = (float)w_size.x / w_size.y;
		if (aspect < target_aspect)
		{
			dst_size.x = w_size.x;
			dst_size.y = w_size.x / target_aspect;
			dst_offset.x = 0;
			dst_offset.y = (w_size.y - dst_size.y) / 2;
		}
		else if (aspect > target_aspect) 
		{
			dst_size.y = w_size.y;
			dst_size.x = w_size.y * target_aspect;
			dst_offset.x = (w_size.x - dst_size.x) / 2;
			dst_offset.y = 0;
		}


		glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer->Handle());
		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachment);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBlitFramebuffer(0, 0, f_size.x, f_size.y, dst_offset.x, dst_offset.y, dst_offset.x + dst_size.x, dst_offset.y + dst_size.y, GL_COLOR_BUFFER_BIT, filter);
	}
	void Window::Present() { glfwSwapBuffers(m_window_handle); }
	void Window::SetCursorLock(bool locked) { glfwSetInputMode(m_window_handle, GLFW_CURSOR, locked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL); }
	void Window::OnEvent(Event& evt)
	{
		if (m_event_callback)
			m_event_callback(evt);
	}
}