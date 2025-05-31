#include "window_events.h"

namespace sugoma::core
{
	std::string WindowEvent::ToString() const { return "WindowEvent"; }
	std::string WindowCloseEvent::ToString() const { return "WindowCloseEvent"; }
	std::string WindowResizeEvent::ToString() const { return "WindowResizeEvent : " + std::to_string(m_Width) + " " + std::to_string(m_Height); }
	std::string WindowMoveEvent::ToString() const { return "WindowMoveEvent : " + std::to_string(m_X) + " " + std::to_string(m_Y); }
	std::string WindowFocusEvent::ToString() const { return "WindowFocusEvent : " + std::to_string(m_focussed); }
}