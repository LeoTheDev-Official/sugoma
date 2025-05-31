#include "event_dispatcher.h"

namespace sugoma::core
{
	EventDispatcher::EventDispatcher(Event& evt) : m_event(&evt) {}
}