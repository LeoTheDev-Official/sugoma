#pragma once
#include "event.h"

namespace sugoma::core
{
	class WindowEvent : public Event
	{
	public:
		std::string ToString() const override;
	};
	class WindowCloseEvent : public WindowEvent 
	{
	public:
		std::string ToString() const override;
	};
	class WindowResizeEvent : public WindowEvent
	{
	public:
		WindowResizeEvent() = delete;
		WindowResizeEvent(uint32_t width, uint32_t height) : m_Width(width), m_Height(height) {}
		std::string ToString() const override;
		inline uint32_t Width() const { return m_Width; }
		inline uint32_t Height() const { return m_Height; }
	private:
		uint32_t m_Width, m_Height;
	};
	class WindowMoveEvent : public WindowEvent 
	{
	public:
		WindowMoveEvent() = delete;
		WindowMoveEvent(uint32_t x, uint32_t y) : m_X(x), m_Y(y) {}
		std::string ToString() const override;
		inline uint32_t X() const { return m_X; }
		inline uint32_t Y() const { return m_Y; }
	private:
		uint32_t m_X, m_Y;
	};
	class WindowFocusEvent : public WindowEvent
	{
	public:
		WindowFocusEvent() = delete;
		WindowFocusEvent(bool focussed) : m_focussed(focussed) {}
		std::string ToString() const override;
		inline bool Focussed() const { return m_focussed; }
	private:
		bool m_focussed;
	};

}