#include "input_system.h"
#include "events/event.h"
#include "events/event_dispatcher.h"
#include "events/input_events.h"

namespace sugoma 
{
	using namespace core;
	enum state_flag : uint8_t
	{
		none = 0,
		pressed = (1 << 0),
		pressed_this_frame = (1 << 1),
		released_this_frame = (1 << 2)
	};
	static uint8_t __key_states[0xFF] = {none};
	static uint8_t __button_states[0xF] = {none};

	static glm::ivec2 __scroll, __mouse_pos, __mouse_delta;


	bool InputSystem::GetKey(KeyCode code) { return __key_states[(KeyID)code] & pressed; }

	bool InputSystem::GetKeyDown(KeyCode code) { return __key_states[(KeyID)code] & pressed_this_frame; }

	bool InputSystem::GetKeyUp(KeyCode code) { return __key_states[(KeyID)code] & released_this_frame; }

	bool InputSystem::GetButton(MouseButton button) { return __button_states[(MouseButtonID)button] & pressed; }

	bool InputSystem::GetButtonDown(MouseButton button) { return __button_states[(MouseButtonID)button] & pressed_this_frame; }

	bool InputSystem::GetButtonUp(MouseButton button) { return __button_states[(MouseButtonID)button] & released_this_frame; }

	glm::vec2 InputSystem::Scroll() { return __scroll; }

	glm::vec2 InputSystem::MousePosition() { return __mouse_pos; }

	glm::vec2 InputSystem::MouseDelta() { return __mouse_delta; }


	bool __kde(KeyDownEvent& evt) 
	{
		auto& state = __key_states[evt.GetKeyCode()];
		state |= pressed | pressed_this_frame;
		return false;
	}
	bool __kue(KeyUpEvent& evt) 
	{
		auto& state = __key_states[evt.GetKeyCode()];
		state = released_this_frame;
		return false;
	}
	bool __mde(MouseButtonDownEvent& evt) 
	{
		auto& state = __button_states[evt.GetButton()];
		state |= pressed | pressed_this_frame;
		return false;
	}
	bool __mue(MouseButtonUpEvent& evt) 
	{
		auto& state = __button_states[evt.GetButton()];
		state = released_this_frame;
		return false;
	}
	bool __sce(MouseScrollEvent& evt) { __scroll = evt.GetScroll(); return false; }
	bool __mme(MouseMovedEvent& evt) 
	{
		__mouse_delta = evt.GetPosition() - __mouse_pos;
		__mouse_pos = evt.GetPosition();
		return false;
	}




	bool InputSystem::SendEvent(Event& event)
	{
		EventDispatcher disp(event);
		disp.Dispatch<KeyDownEvent>(__kde);
		disp.Dispatch<KeyUpEvent>(__kue);
		disp.Dispatch<MouseButtonDownEvent>(__mde);
		disp.Dispatch<MouseButtonUpEvent>(__mue);
		disp.Dispatch<MouseScrollEvent>(__sce);
		disp.Dispatch<MouseMovedEvent>(__mme);
		return false;
	}

	void InputSystem::Tick()
	{
		for (auto& k : __key_states) k &= pressed;
		for (auto& b : __button_states) b &= pressed;
		__mouse_delta = { 0, 0 };
		__scroll = { 0, 0 };
	}

}