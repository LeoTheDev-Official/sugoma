#pragma once
#include <cstdint>
#include "event.h"
#include "glm/glm.hpp"
namespace sugoma::core
{
#pragma region keyboard
	using KeyID = uint32_t;
	class InputEvent : public Event 
	{
	public:
		std::string ToString() const override;
	};
	class KeyEvent : public InputEvent
	{
	public:
		KeyEvent() = delete;
		KeyEvent(KeyID keycode);
		KeyID GetKeyCode();
		std::string ToString() const override;
	protected:
		KeyID m_keycode;
	};
	class KeyDownEvent : public KeyEvent
	{
	public:
		KeyDownEvent(KeyID keycode);
		std::string ToString() const override;
	};
	class KeyUpEvent : public KeyEvent 
	{
	public:
		KeyUpEvent(KeyID keycode);
		std::string ToString() const override;
	};
	class KeyRepeatEvent : public KeyEvent
	{
	public:
		KeyRepeatEvent(KeyID keycode);
		std::string ToString() const override;
	};
#pragma endregion

#pragma region mouse
	using MouseButtonID = uint8_t;
	class MouseEvent : public InputEvent 
	{
	public:
		std::string ToString() const override;
	};
	class MouseMovedEvent : public MouseEvent 
	{
	public:
		MouseMovedEvent() = delete;
		MouseMovedEvent(glm::ivec2 position);
		glm::ivec2 GetPosition() const;
		std::string ToString() const override;
	protected:
		glm::ivec2 m_position;
	};
	class MouseScrollEvent : public MouseEvent 
	{
	public:
		MouseScrollEvent() = delete;
		MouseScrollEvent(glm::ivec2 scroll);
		glm::ivec2 GetScroll() const;
		std::string ToString() const override;
	protected:
		glm::ivec2 m_scroll;
	};
	class MouseButtonEvent : public MouseEvent
	{
	public:
		MouseButtonEvent() = delete;
		MouseButtonEvent(MouseButtonID btn);
		MouseButtonID GetButton() const;
		std::string ToString() const override;
	protected:
		MouseButtonID m_button;
	};
	class MouseButtonDownEvent : public MouseButtonEvent 
	{
	public:
		MouseButtonDownEvent(MouseButtonID btn);
		std::string ToString() const override;
	};
	class MouseButtonUpEvent : public MouseButtonEvent 
	{
	public:
		MouseButtonUpEvent(MouseButtonID btn);
		std::string ToString() const override;
	};
#pragma endregion


}