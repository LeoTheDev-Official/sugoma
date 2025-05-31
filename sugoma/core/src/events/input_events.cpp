#include "input_events.h"
#include "glm/gtx/string_cast.hpp"

namespace sugoma::core
{
	KeyEvent::KeyEvent(KeyID keycode) : m_keycode(keycode){}
	KeyID KeyEvent::GetKeyCode() { return m_keycode; }
	std::string KeyEvent::ToString() const { return "KeyEvent : " + std::to_string(m_keycode); }
	KeyDownEvent::KeyDownEvent(KeyID keycode) : KeyEvent(keycode) {}
	std::string KeyDownEvent::ToString() const { return "KeyDownEvent : " + std::to_string(m_keycode); }
	KeyUpEvent::KeyUpEvent(KeyID keycode) : KeyEvent(keycode) {}
	std::string KeyUpEvent::ToString() const { return "KeyUpEvent : " + std::to_string(m_keycode); }
	KeyRepeatEvent::KeyRepeatEvent(KeyID keycode) : KeyEvent(keycode) {}
	std::string KeyRepeatEvent::ToString() const { return "KeyRepeatEvent : " + std::to_string(m_keycode); }
	std::string MouseEvent::ToString() const { return "MouseEvent"; }
	MouseMovedEvent::MouseMovedEvent(glm::ivec2 position) : m_position(position) {}
	glm::ivec2 MouseMovedEvent::GetPosition() const { return m_position; }
	std::string MouseMovedEvent::ToString() const { return "MouseMovedEvent : " + glm::to_string(m_position); }
	MouseScrollEvent::MouseScrollEvent(glm::ivec2 scroll) : m_scroll(scroll) {}
	glm::ivec2 MouseScrollEvent::GetScroll() const { return m_scroll; }
	std::string MouseScrollEvent::ToString() const { return "MouseScrollEvent : " + glm::to_string(m_scroll); }
	MouseButtonEvent::MouseButtonEvent(MouseButtonID btn) : m_button(btn) {}
	MouseButtonID MouseButtonEvent::GetButton() const { return m_button; }
	std::string MouseButtonEvent::ToString() const { return "MouseButtonEvent : " + std::to_string(m_button); }
	MouseButtonDownEvent::MouseButtonDownEvent(MouseButtonID btn) : MouseButtonEvent(btn) {}
	std::string MouseButtonDownEvent::ToString() const { return "MouseButtonDownEvent : " + std::to_string(m_button); }
	MouseButtonUpEvent::MouseButtonUpEvent(MouseButtonID btn) : MouseButtonEvent(btn) {}
	std::string MouseButtonUpEvent::ToString() const { return "MouseButtonUpEvent : " + std::to_string(m_button); }
	std::string InputEvent::ToString() const { return "InputEvent"; }
}